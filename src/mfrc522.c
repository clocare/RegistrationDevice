/*
 *  @file mfrc522.c
 *
 *  @description
 *
 *  @date: 20.11.2016
 *  @author: Michael Sauer <sauer.uetersen@gmail.com>
 */

/* Includes ------------------------------------------------------------------*/
#include "STD_TYPES.h"

#include "DIO_interface.h"
#include "SPI1.h"
#include "TIMER_interface.h"

#include "mfrc522.h"

#define NULL_PTR          ((void *)0)

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
// #define MGPIO_VidSetPinValue(mfrc522_CS_PORT , mfrc522_CS_PIN , LOW);	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
// #define MGPIO_VidSetPinValue(mfrc522_CS_PORT , mfrc522_CS_PIN , HIGH);	GPIO_SetBits(GPIOA, GPIO_Pin_4);
#undef mfrc522_DumpToSerial
/* Private variables ---------------------------------------------------------*/
static Uid uid;
/* Private function prototypes -----------------------------------------------*/
u8 MIFARE_TwoStepHelper(u8 command, u8 blockAddr,	long data);
void PCD_Init_IO(void);
/* Private functions ---------------------------------------------------------*/

/**
 * Helper function for the two-step MIFARE Classic protocol operations Decrement, Increment and Restore.
 *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise.
 */
u8 MIFARE_TwoStepHelper(	u8 command,	///< The command to use
									u8 blockAddr,	///< The block (0-0xff) number.
									long data		///< The data to transfer in step 2
									) {
	u8 result;
	u8 cmdBuffer[2]; // We only need room for 2 bytes.

	// Step 1: Tell the PICC the command and block address
	cmdBuffer[0] = command;
	cmdBuffer[1] = blockAddr;
	result = PCD_MIFARE_Transceive(	cmdBuffer, 2, FALSE); // Adds CRC_A and checks that the response is MF_ACK.
	if (result != mfrc522_STATUS_OK) {
		return result;
	}

	// Step 2: Transfer the data
	result = PCD_MIFARE_Transceive(	(u8 *)&data, 4, TRUE); // Adds CRC_A and accept timeout as success.
	if (result != mfrc522_STATUS_OK) {
		return result;
	}

	return mfrc522_STATUS_OK;
} // End MIFARE_TwoStepHelper()
/* Public functions ----------------------------------------------------------*/
void PCD_WriteRegister(	u8 reg,		///< The register to write to. One of the PCD_Register enums.
									u16 value		///< The value to write.
								) {
		// Select slave
		MGPIO_VidSetPinValue(mfrc522_CS_PORT , mfrc522_CS_PIN , LOW);
		SPI1_u8SendRecieveSync((reg & 0x7E));	// MSB == 0 is for writing. LSB is not used in address. Datasheet section 8.1.2.3.
		SPI1_u8SendRecieveSync(value);
		// Release slave again
		MGPIO_VidSetPinValue(mfrc522_CS_PORT , mfrc522_CS_PIN , HIGH);
} // End PCD_WriteRegister()

/**
 * Writes a number of bytes to the specified register in the MFRC522 chip.
 * The interface is described in the datasheet section 8.1.2.
 */
void PCD_WriteRegisterMulti(	u8 reg,		///< The register to write to. One of the PCD_Register enums.
									u8 count,		///< The number of bytes to write to the register
									u8 *values	///< The values to write. Byte array.
								) {
	// Select slave
	MGPIO_VidSetPinValue(mfrc522_CS_PORT , mfrc522_CS_PIN , LOW);
	//SPI1_u8SendRecieveSync(reg & 0x7E);				// MSB == 0 is for writing. LSB is not used in address. Datasheet section 8.1.2.3.
	for (u8 index = 0; index < count; index++) {
		SPI1_u8SendRecieveSync(values[index]);
	}
	MGPIO_VidSetPinValue(mfrc522_CS_PORT , mfrc522_CS_PIN , HIGH);	// Release slave again
} // End PCD_WriteRegister()

/**
 * Reads a u8 from the specified register in the MFRC522 chip.
 * The interface is described in the datasheet section 8.1.2.
 */
u8 PCD_ReadRegister(	u8 reg	///< The register to read from. One of the PCD_Register enums.
								) {
	volatile u8 value;

	MGPIO_VidSetPinValue(mfrc522_CS_PORT , mfrc522_CS_PIN , LOW);
	u8 address = 0x80 | (reg & 0x7E);// Select slave
	value = SPI1_u8SendRecieveSync(address);			// MSB == 1 is for reading. LSB is not used in address. Datasheet section 8.1.2.3.
	value = SPI1_u8SendRecieveSync(0);					// Read the value back. Send 0 to stop reading.
	MGPIO_VidSetPinValue(mfrc522_CS_PORT , mfrc522_CS_PIN , HIGH);			// Release slave again
	return (value);
} // End PCD_ReadRegister()

/**
 * Reads a number of bytes from the specified register in the MFRC522 chip.
 * The interface is described in the datasheet section 8.1.2.
 */
void PCD_ReadRegisterMulti(	u8 reg,		///< The register to read from. One of the PCD_Register enums.
								u8 count,		///< The number of bytes to read
								u8 *values,	///< Byte array to store the values in.
								u8 rxAlign	///< Only bit positions rxAlign..7 in values[0] are updated.
								) {
	if (count == 0) {
		return;
	}
	//printf("Reading %d bytes from register.", count);
	u8 address = 0x80 | (reg & 0x7E);		// MSB == 1 is for reading. LSB is not used in address. Datasheet section 8.1.2.3.
	u8 index = 0;							// Index in values array.
	MGPIO_VidSetPinValue(mfrc522_CS_PORT , mfrc522_CS_PIN , LOW);		// Select slave
	count--;								// One read is performed outside of the loop
	SPI1_u8SendRecieveSync(address);					// Tell MFRC522 which address we want to read
	while (index < count) {
		if (index == 0 && rxAlign) { // Only update bit positions rxAlign..7 in values[0]
			// Create bit mask for bit positions rxAlign..7
			u8 mask = 0;
			for (u8 i = rxAlign; i <= 7; i++) {
				mask |= (1 << i);
			}
			// Read value and tell that we want to read the same address again.
			u16 value = SPI1_u8SendRecieveSync(address);
			// Apply mask to both current value of values[0] and the new data in value.
			values[0] = (values[index] & ~mask) | (value & mask);
		}
		else { // Normal case
			values[index] = (SPI1_u8SendRecieveSync(address)) & 0xFF;	// Read value and tell that we want to read the same address again.
		}
		index++;
	}
	values[index] = SPI1_u8SendRecieveSync(0) & 0xFF;			// Read the final u8. Send 0 to stop reading.
	MGPIO_VidSetPinValue(mfrc522_CS_PORT , mfrc522_CS_PIN , HIGH);			// Release slave again
} // End PCD_ReadRegister()

/**
 * Sets the bits given in mask in register reg.
 */
void PCD_SetRegisterBitMask(	u8 reg,	///< The register to update. One of the PCD_Register enums.
										u8 mask	///< The bits to set.
									) {
	u8 tmp;
	tmp = PCD_ReadRegister(reg);
	PCD_WriteRegister(reg, tmp | mask);			// set bit mask
} // End PCD_SetRegisterBitMask()

/**
 * Clears the bits given in mask from register reg.
 */
void PCD_ClearRegisterBitMask(	u8 reg,	///< The register to update. One of the PCD_Register enums.
										u8 mask	///< The bits to clear.
									  ) {
	u8 tmp;
	tmp = PCD_ReadRegister(reg);
	PCD_WriteRegister(reg, tmp & (~mask));		// clear bit mask
} // End PCD_ClearRegisterBitMask()


/**
 * Use the CRC coprocessor in the MFRC522 to calculate a CRC_A.
 *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise.
 */
u8 PCD_CalculateCRC(	u8 *data,		///< In: Pointer to the data to transfer to the FIFO for CRC calculation.
								u8 length,	///< In: The number of bytes to transfer.
								u8 *result	///< Out: Pointer to result buffer. Result is written to result[0..1], low u8 first.
					 ) {
	PCD_WriteRegister(CommandReg, PCD_Idle);			// Stop any active command.
	PCD_WriteRegister(DivIrqReg, 0x04);					// Clear the CRCIRq interrupt request bit
	PCD_SetRegisterBitMask(FIFOLevelReg, 0x80);		// FlushBuffer = 1, FIFO initialization
	PCD_WriteRegisterMulti(FIFODataReg, length, data);		// Write data to the FIFO
	PCD_WriteRegister(CommandReg, PCD_CalcCRC);		// Start the calculation

	// Wait for the CRC calculation to complete. Each iteration of the while-loop takes 17.73�s.
	u16 i = 5000;
	u8 n;
	while (1) {
		n = PCD_ReadRegister(DivIrqReg);	// DivIrqReg[7..0] bits are: Set2 reserved reserved MfinActIRq   reserved CRCIRq reserved reserved
		if (n & 0x04) {						// CRCIRq bit set - calculation done
			break;
		}
		if (--i == 0) {						// The emergency break. We will eventually terminate on this one after 89ms. Communication with the MFRC522 might be down.
			return mfrc522_STATUS_TIMEOUT;
		}
	}
	PCD_WriteRegister(CommandReg, PCD_Idle);			// Stop calculating CRC for new content in the FIFO.

	// Transfer the result from the registers to the result buffer
	result[0] = PCD_ReadRegister(CRCResultRegL);
	result[1] = PCD_ReadRegister(CRCResultRegH);
	return mfrc522_STATUS_OK;
} // End PCD_CalculateCRC()


/////////////////////////////////////////////////////////////////////////////////////
// Functions for manipulating the MFRC522
/////////////////////////////////////////////////////////////////////////////////////

/**
 * Initializes the MFRC522 chip.
 */
void PCD_Init(void) {
	
/* To be handled by Main 					*/
	//	PCD_Init_IO(); // Init IO Pins
	//	SPI1_Init(); // Init SPI1
/**********************************/
	
//  Perform a soft reset
	PCD_Reset();

	// When communicating with a PICC we need a timeout if something goes wrong.
	// f_timer = 13.56 MHz / (2*TPreScaler+1) where TPreScaler = [TPrescaler_Hi:TPrescaler_Lo].
	// TPrescaler_Hi are the four low bits in TModeReg. TPrescaler_Lo is TPrescalerReg.
    PCD_WriteRegister(TModeReg, 0x80);			// TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
    PCD_WriteRegister(TPrescalerReg, 0xA9);	// TPreScaler = TModeReg[3..0]:TPrescalerReg, ie 0x0A9 = 169 => f_timer=40kHz, ie a timer period of 25�s.
    PCD_WriteRegister(TReloadRegH, 0x03);		// Reload timer with 0x3E8 = 1000, ie 25ms before timeout.
    PCD_WriteRegister(TReloadRegL, 0xE8);

	PCD_WriteRegister(TxASKReg, 0x40);		// Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
	PCD_WriteRegister(ModeReg, 0x3D);		// Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)
	PCD_AntennaOn();						// Enable the antenna driver pins TX1 and TX2 (they were disabled by the reset)
} // End PCD_Init()

/**
 * Performs a soft reset on the MFRC522 chip and waits for it to be ready again.
 */
void PCD_Reset(void) {
	PCD_WriteRegister(CommandReg, PCD_SoftReset);	// Issue the SoftReset command.
	// The datasheet does not mention how long the SoftRest command takes to complete.
	// But the MFRC522 might have been in soft power-down mode (triggered by bit 4 of CommandReg)
	// Section 8.8.2 in the datasheet says the oscillator start-up time is the start up time of the crystal + 37,74�s. Let us be generous: 50ms.
	TIMER_voidSetBusyWait(TIM3 ,50000);
//	DWT_Delay_ms(50);
	// Wait for the PowerDown bit in CommandReg to be cleared
	while (PCD_ReadRegister(CommandReg) & (1<<4)) {
		// PCD still restarting - unlikely after waiting 50ms, but better safe than sorry.
	}
} // End PCD_Reset()

/**
 * Turns the antenna on by enabling pins TX1 and TX2.
 * After a reset these pins disabled.
 */
void PCD_AntennaOn(void) {
	u8 value = PCD_ReadRegister(TxControlReg);
	if ((value & 0x03) != 0x03) {
		PCD_WriteRegister(TxControlReg, value | 0x03);
	}
} // End PCD_AntennaOn()

void PCD_AntennaOff() {
	PCD_ClearRegisterBitMask(TxControlReg, 0x03);
} // End PCD_AntennaOff()

/////////////////////////////////////////////////////////////////////////////////////
// Functions for communicating with PICCs
/////////////////////////////////////////////////////////////////////////////////////

/**
 * Executes the Transceive command.
 * CRC validation can only be done if backData and backLen are specified.
 *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise.
 */
u8 PCD_TransceiveData(	u8 *sendData,		///< Pointer to the data to transfer to the FIFO.
									u8 sendLen,		///< Number of bytes to transfer to the FIFO.
									u8 *backData,		///< NULL_PTR or pointer to buffer if data should be read back after executing the command.
									u8 *backLen,		///< In: Max number of bytes to write to *backData. Out: The number of bytes returned.
									u8 *validBits,	///< In/Out: The number of valid bits in the last u8. 0 for 8 valid bits. Default NULL_PTR.
									u8 rxAlign,		///< In: Defines the bit position in backData[0] for the first bit received. Default 0.
									boolean checkCRC		///< In: True => The last two bytes of the response is assumed to be a CRC_A that must be validated.
								 ) {
	u8 waitIRq = 0x30;		// RxIRq and IdleIRq
	return PCD_CommunicateWithPICC(PCD_Transceive, waitIRq, sendData, sendLen, backData, backLen, validBits, rxAlign, checkCRC);
} // End PCD_TransceiveData()

/**
 * Transfers data to the MFRC522 FIFO, executes a commend, waits for completion and transfers data back from the FIFO.
 * CRC validation can only be done if backData and backLen are specified.
 *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise.
 */
u8 PCD_CommunicateWithPICC(	u8 command,		///< The command to execute. One of the PCD_Command enums.
										u8 waitIRq,		///< The bits in the ComIrqReg register that signals successful completion of the command.
										u8 *sendData,		///< Pointer to the data to transfer to the FIFO.
										u8 sendLen,		///< Number of bytes to transfer to the FIFO.
										u8 *backData,		///< NULL_PTR or pointer to buffer if data should be read back after executing the command.
										u8 *backLen,		///< In: Max number of bytes to write to *backData. Out: The number of bytes returned.
										u8 *validBits,	///< In/Out: The number of valid bits in the last u8. 0 for 8 valid bits.
										u8 rxAlign,		///< In: Defines the bit position in backData[0] for the first bit received. Default 0.
										boolean checkCRC		///< In: True => The last two bytes of the response is assumed to be a CRC_A that must be validated.
									 ) {
	u8 n, _validBits;
	unsigned int i;

	// Prepare values for BitFramingReg
	u8 txLastBits = validBits ? *validBits : 0;
	u8 bitFraming	= (u8)(rxAlign << 4) + txLastBits;		// RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

	PCD_WriteRegister(CommandReg, PCD_Idle);			// Stop any active command.
	PCD_WriteRegister(ComIrqReg, 0x7F);					// Clear all seven interrupt request bits
	PCD_SetRegisterBitMask(FIFOLevelReg, 0x80);		// FlushBuffer = 1, FIFO initialization
	PCD_WriteRegisterMulti(FIFODataReg, sendLen, sendData);	// Write sendData to the FIFO
	PCD_WriteRegister(BitFramingReg, bitFraming);		// Bit adjustments
	PCD_WriteRegister(CommandReg, command);			// Execute the command
	if (command == PCD_Transceive) 	{
		PCD_SetRegisterBitMask(BitFramingReg, 0x80);	// StartSend=1, transmission of data starts
	}

	// Wait for the command to complete.
	// In PCD_Init() we set the TAuto flag in TModeReg. This means the timer automatically starts when the PCD stops transmitting.
	// Each iteration of the do-while-loop takes 17.86�s.
	i = 10000;
	while (1) {
		n = PCD_ReadRegister(ComIrqReg);	// ComIrqReg[7..0] bits are: Set1 TxIRq RxIRq IdleIRq   HiAlertIRq LoAlertIRq ErrIRq TimerIRq
		if (n & waitIRq) {					// One of the interrupts that signal success has been set.
			break;
		}
		if (n & 0x01) {	// Timer interrupt - nothing received in 25ms
			return mfrc522_STATUS_TIMEOUT;
		}
		if (--i == 0) {						// The emergency break. If all other condions fail we will eventually terminate on this one after 35.7ms. Communication with the MFRC522 might be down.
			return mfrc522_STATUS_TIMEOUT;
		}
	}

	// Stop now if any errors except collisions were detected.
	u8 errorRegValue = PCD_ReadRegister(ErrorReg); // ErrorReg[7..0] bits are: WrErr TempErr reserved BufferOvfl   CollErr CRCErr ParityErr ProtocolErr
	if (errorRegValue & 0x13) {	 // BufferOvfl ParityErr ProtocolErr
		return mfrc522_STATUS_ERROR;
	}

	// If the caller wants data back, get it from the MFRC522.
	if (backData && backLen) {
		n = PCD_ReadRegister(FIFOLevelReg);						// Number of bytes in the FIFO
		if (n > *backLen) {
			return mfrc522_STATUS_NO_ROOM;
		}
		*backLen = n;												// Number of bytes returned
		PCD_ReadRegisterMulti(FIFODataReg, n, backData, rxAlign);		// Get received data from FIFO
		_validBits = PCD_ReadRegister(ControlReg) & 0x07;	// RxLastBits[2:0] indicates the number of valid bits in the last received u8. If this value is 000b, the whole u8 is valid.
		if (validBits) {
			*validBits = _validBits;
		}
	}

	// Tell about collisions
	if (errorRegValue & 0x08) { // CollErr
		return mfrc522_STATUS_COLLISION;
	}

	// Perform CRC_A validation if requested.
	if (backData && backLen && checkCRC) {
		// In this case a MIFARE Classic NAK is not OK.
		if (*backLen == 1 && _validBits == 4) {
			return mfrc522_STATUS_MIFARE_NACK;
		}
		// We need at least the CRC_A value and all 8 bits of the last u8 must be received.
		if (*backLen < 2 || _validBits != 0) {
			return mfrc522_STATUS_CRC_WRONG;
		}
		// Verify CRC_A - do our own calculation and store the control in controlBuffer.
		u8 controlBuffer[2];
		n = PCD_CalculateCRC(&backData[0], *backLen - 2, &controlBuffer[0]);
		if (n != mfrc522_STATUS_OK) {
			return n;
		}
		if ((backData[*backLen - 2] != controlBuffer[0]) || (backData[*backLen - 1] != controlBuffer[1])) {
			return mfrc522_STATUS_CRC_WRONG;
		}
	}

	return mfrc522_STATUS_OK;
} // End PCD_CommunicateWithPICC()

/**
 * Transmits a REQuest command, Type A. Invites PICCs in state IDLE to go to READY and prepare for anticollision or selection. 7 bit frame.
 * Beware: When two PICCs are in the field at the same time I often get mfrc522_STATUS_TIMEOUT - probably due do bad antenna design.
 *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise.
 */
u8 PICC_RequestA(u8 *bufferATQA,	///< The buffer to store the ATQA (Answer to request) in
							u8 *bufferSize	///< Buffer size, at least two bytes. Also number of bytes returned if mfrc522_STATUS_OK.
							) {
	return PICC_REQA_or_WUPA(PICC_CMD_REQA, bufferATQA, bufferSize);
} // End PICC_RequestA()

/**
 * Transmits a Wake-UP command, Type A. Invites PICCs in state IDLE and HALT to go to READY(*) and prepare for anticollision or selection. 7 bit frame.
 * Beware: When two PICCs are in the field at the same time I often get mfrc522_STATUS_TIMEOUT - probably due do bad antenna design.
 *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise.
 */
u8 PICC_WakeupA(	u8 *bufferATQA,	///< The buffer to store the ATQA (Answer to request) in
							u8 *bufferSize	///< Buffer size, at least two bytes. Also number of bytes returned if mfrc522_STATUS_OK.
							) {
	return PICC_REQA_or_WUPA(PICC_CMD_WUPA, bufferATQA, bufferSize);
} // End PICC_WakeupA()

/**
 * Transmits REQA or WUPA commands.
 * Beware: When two PICCs are in the field at the same time I often get mfrc522_STATUS_TIMEOUT - probably due do bad antenna design.
 *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise.
 */
u8 PICC_REQA_or_WUPA(	u8 command, 		///< The command to send - PICC_CMD_REQA or PICC_CMD_WUPA
									u8 *bufferATQA,	///< The buffer to store the ATQA (Answer to request) in
									u8 *bufferSize	///< Buffer size, at least two bytes. Also number of bytes returned if mfrc522_STATUS_OK.
							   ) {
	u8 validBits;
	u8 status;

	if (bufferATQA == NULL_PTR || *bufferSize < 2) {	// The ATQA response is 2 bytes long.
		return mfrc522_STATUS_NO_ROOM;
	}
	PCD_ClearRegisterBitMask(CollReg, 0x80);			// ValuesAfterColl=1 => Bits received after collision are cleared.
	validBits = 7;										// For REQA and WUPA we need the short frame format - transmit only 7 bits of the last (and only) u8. TxLastBits = BitFramingReg[2..0]
	status = PCD_TransceiveData(&command, 1, bufferATQA, bufferSize, &validBits, 0, FALSE);
	if (status != mfrc522_STATUS_OK) {
		return status;
	}
	if (*bufferSize != 2 || validBits != 0) {		// ATQA must be exactly 16 bits.
		return mfrc522_STATUS_ERROR;
	}
	return mfrc522_STATUS_OK;
} // End PICC_REQA_or_WUPA()

/**
 * Transmits SELECT/ANTICOLLISION commands to select a single PICC.
 * Before calling this function the PICCs must be placed in the READY(*) state by calling PICC_RequestA() or PICC_WakeupA().
 * On success:
 * 		- The chosen PICC is in state ACTIVE(*) and all other PICCs have returned to state IDLE/HALT. (Figure 7 of the ISO/IEC 14443-3 draft.)
 * 		- The UID size and value of the chosen PICC is returned in *uid along with the SAK.
 *
 * A PICC UID consists of 4, 7 or 10 bytes.
 * Only 4 bytes can be specified in a SELECT command, so for the longer UIDs two or three iterations are used:
 * 		UID size	Number of UID bytes		Cascade levels		Example of PICC
 * 		========	===================		==============		===============
 * 		single				 4						1				MIFARE Classic
 * 		double				 7						2				MIFARE Ultralight
 * 		triple				10						3				Not currently in use?
 *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise.
 */
u8 PICC_Select(	Uid *uid,			///< Pointer to Uid struct. Normally output, but can also be used to supply a known UID.
							u8 validBits		///< The number of known UID bits supplied in *uid. Normally 0. If set you must also supply uid->size.
						 ) {
	boolean uidComplete;
	boolean selectDone;
	boolean useCascadeTag;
	u8 cascadeLevel	= 1;
	u8 result;
	u8 count;
	u8 index;
	u8 uidIndex;					// The first index in uid->uidByte[] that is used in the current Cascade Level.
	sint8 currentLevelKnownBits;		// The number of known UID bits in the current Cascade Level.
	u8 buffer[9];					// The SELECT/ANTICOLLISION commands uses a 7 u8 standard frame + 2 bytes CRC_A
	u8 bufferUsed;				// The number of bytes used in the buffer, ie the number of bytes to transfer to the FIFO.
	u8 rxAlign;					// Used in BitFramingReg. Defines the bit position for the first bit received.
	u8 txLastBits;				// Used in BitFramingReg. The number of valid bits in the last transmitted u8.
	u8 *responseBuffer;
	u8 responseLength;

	// Description of buffer structure:
	// 		Byte 0: SEL 				Indicates the Cascade Level: PICC_CMD_SEL_CL1, PICC_CMD_SEL_CL2 or PICC_CMD_SEL_CL3
	// 		Byte 1: NVB					Number of Valid Bits (in complete command, not just the UID): High nibble: complete bytes, Low nibble: Extra bits.
	// 		Byte 2: UID-data or CT		See explanation below. CT means Cascade Tag.
	// 		Byte 3: UID-data
	// 		Byte 4: UID-data
	// 		Byte 5: UID-data
	// 		Byte 6: BCC					Block Check Character - XOR of bytes 2-5
	//		Byte 7: CRC_A
	//		Byte 8: CRC_A
	// The BCC and CRC_A is only transmitted if we know all the UID bits of the current Cascade Level.
	//
	// Description of bytes 2-5: (Section 6.5.4 of the ISO/IEC 14443-3 draft: UID contents and cascade levels)
	//		UID size	Cascade level	Byte2	Byte3	Byte4	Byte5
	//		========	=============	=====	=====	=====	=====
	//		 4 bytes		1			uid0	uid1	uid2	uid3
	//		 7 bytes		1			CT		uid0	uid1	uid2
	//						2			uid3	uid4	uid5	uid6
	//		10 bytes		1			CT		uid0	uid1	uid2
	//						2			CT		uid3	uid4	uid5
	//						3			uid6	uid7	uid8	uid9

	// Sanity checks
	if (validBits > 80) {
		return mfrc522_STATUS_INVALID;
	}

	// Prepare MFRC522
	PCD_ClearRegisterBitMask(CollReg, 0x80);			// ValuesAfterColl=1 => Bits received after collision are cleared.

	// Repeat Cascade Level loop until we have a complete UID.
	uidComplete = FALSE;
	while ( ! uidComplete) {
		// Set the Cascade Level in the SEL u8, find out if we need to use the Cascade Tag in u8 2.
		switch (cascadeLevel) {
			case 1:
				buffer[0] = PICC_CMD_SEL_CL1;
				uidIndex = 0;
				useCascadeTag = validBits && uid->size > 4;	// When we know that the UID has more than 4 bytes
				break;

			case 2:
				buffer[0] = PICC_CMD_SEL_CL2;
				uidIndex = 3;
				useCascadeTag = validBits && uid->size > 7;	// When we know that the UID has more than 7 bytes
				break;

			case 3:
				buffer[0] = PICC_CMD_SEL_CL3;
				uidIndex = 6;
				useCascadeTag = FALSE;						// Never used in CL3.
				break;

			default:
				return mfrc522_STATUS_INTERNAL_ERROR;
				break;
		}

		// How many UID bits are known in this Cascade Level?
		currentLevelKnownBits = validBits - (8 * uidIndex);
		if (currentLevelKnownBits < 0) {
			currentLevelKnownBits = 0;
		}
		// Copy the known bits from uid->uidByte[] to buffer[]
		index = 2; // destination index in buffer[]
		if (useCascadeTag) {
			buffer[index++] = PICC_CMD_CT;
		}
		u8 bytesToCopy = currentLevelKnownBits / 8 + (currentLevelKnownBits % 8 ? 1 : 0); // The number of bytes needed to represent the known bits for this level.
		if (bytesToCopy) {
			u8 maxBytes = useCascadeTag ? 3 : 4; // Max 4 bytes in each Cascade Level. Only 3 left if we use the Cascade Tag
			if (bytesToCopy > maxBytes) {
				bytesToCopy = maxBytes;
			}
			for (count = 0; count < bytesToCopy; count++) {
				buffer[index++] = uid->uidByte[uidIndex + count];
			}
		}
		// Now that the data has been copied we need to include the 8 bits in CT in currentLevelKnownBits
		if (useCascadeTag) {
			currentLevelKnownBits += 8;
		}

		// Repeat anti collision loop until we can transmit all UID bits + BCC and receive a SAK - max 32 iterations.
		selectDone = FALSE;
		while ( ! selectDone) {
			// Find out how many bits and bytes to send and receive.
			if (currentLevelKnownBits >= 32) { // All UID bits in this Cascade Level are known. This is a SELECT.
				//printf("SELECT: currentLevelKnownBits=%d", currentLevelKnownBits);
				buffer[1] = 0x70; // NVB - Number of Valid Bits: Seven whole bytes
				// Calulate BCC - Block Check Character
				buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
				// Calculate CRC_A
				result = PCD_CalculateCRC(buffer, 7, &buffer[7]);
				if (result != mfrc522_STATUS_OK) {
					return result;
				}
				txLastBits		= 0; // 0 => All 8 bits are valid.
				bufferUsed		= 9;
				// Store response in the last 3 bytes of buffer (BCC and CRC_A - not needed after tx)
				responseBuffer	= &buffer[6];
				responseLength	= 3;
			}
			else { // This is an ANTICOLLISION.
				//printf("ANTICOLLISION: currentLevelKnownBits=%d", currentLevelKnownBits);
				txLastBits		= currentLevelKnownBits % 8;
				count			= currentLevelKnownBits / 8;	// Number of whole bytes in the UID part.
				index			= 2 + count;					// Number of whole bytes: SEL + NVB + UIDs
				buffer[1]		= (index << 4) + txLastBits;	// NVB - Number of Valid Bits
				bufferUsed		= index + (txLastBits ? 1 : 0);
				// Store response in the unused part of buffer
				responseBuffer	= &buffer[index];
				responseLength	= sizeof(buffer) - index;
			}

			// Set bit adjustments
			rxAlign = txLastBits;											// Having a seperate variable is overkill. But it makes the next line easier to read.
			PCD_WriteRegister(BitFramingReg, (rxAlign << 4) + txLastBits);	// RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

			// Transmit the buffer and receive the response.
			result = PCD_TransceiveData(buffer, bufferUsed, responseBuffer, &responseLength, &txLastBits, rxAlign, FALSE);
			if (result == mfrc522_STATUS_COLLISION) { // More than one PICC in the field => collision.
				result = PCD_ReadRegister(CollReg); // CollReg[7..0] bits are: ValuesAfterColl reserved CollPosNotValid CollPos[4:0]
				if (result & 0x20) { // CollPosNotValid
					return mfrc522_STATUS_COLLISION; // Without a valid collision position we cannot continue
				}
				u8 collisionPos = result & 0x1F; // Values 0-31, 0 means bit 32.
				if (collisionPos == 0) {
					collisionPos = 32;
				}
				if (collisionPos <= currentLevelKnownBits) { // No progress - should not happen
					return mfrc522_STATUS_INTERNAL_ERROR;
				}
				// Choose the PICC with the bit set.
				currentLevelKnownBits = collisionPos;
				count			= (currentLevelKnownBits - 1) % 8; // The bit to modify
				index			= 1 + (currentLevelKnownBits / 8) + (count ? 1 : 0); // First u8 is index 0.
				buffer[index]	|= (1 << count);
			}
			else if (result != mfrc522_STATUS_OK) {
				return result;
			}
			else { // mfrc522_STATUS_OK
				if (currentLevelKnownBits >= 32) { // This was a SELECT.
					selectDone = TRUE; // No more anticollision
					// We continue below outside the while.
				}
				else { // This was an ANTICOLLISION.
					// We now have all 32 bits of the UID in this Cascade Level
					currentLevelKnownBits = 32;
					// Run loop again to do the SELECT.
				}
			}
		} // End of while ( ! selectDone)

		// We do not check the CBB - it was constructed by us above.

		// Copy the found UID bytes from buffer[] to uid->uidByte[]
		index			= (buffer[2] == PICC_CMD_CT) ? 3 : 2; // source index in buffer[]
		bytesToCopy		= (buffer[2] == PICC_CMD_CT) ? 3 : 4;
		for (count = 0; count < bytesToCopy; count++) {
			uid->uidByte[uidIndex + count] = buffer[index++];
		}

		// Check response SAK (Select Acknowledge)
		if (responseLength != 3 || txLastBits != 0) {		// SAK must be exactly 24 bits (1 u8 + CRC_A).
			return mfrc522_STATUS_ERROR;
		}
		// Verify CRC_A - do our own calculation and store the control in buffer[2..3] - those bytes are not needed anymore.
		result = PCD_CalculateCRC(responseBuffer, 1, &buffer[2]);
		if (result != mfrc522_STATUS_OK) {
			return result;
		}
		if ((buffer[2] != responseBuffer[1]) || (buffer[3] != responseBuffer[2])) {
			return mfrc522_STATUS_CRC_WRONG;
		}
		if (responseBuffer[0] & 0x04) { // Cascade bit set - UID not complete yes
			cascadeLevel++;
		}
		else {
			uidComplete = TRUE;
			uid->sak = responseBuffer[0];
		}
	} // End of while ( ! uidComplete)

	// Set correct uid->size
	uid->size = 3 * cascadeLevel + 1;

	return mfrc522_STATUS_OK;
} // End PICC_Select()

/**
 * Instructs a PICC in state ACTIVE(*) to go to state HALT.
 *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise.
 */
u8 PICC_HaltA() {
	u8 result;
	u8 buffer[4];

	// Build command buffer
	buffer[0] = PICC_CMD_HLTA;
	buffer[1] = 0;
	// Calculate CRC_A
	result = PCD_CalculateCRC(buffer, 2, &buffer[2]);
	if (result != mfrc522_STATUS_OK) {
		return result;
	}

	// Send the command.
	// The standard says:
	//		If the PICC responds with any modulation during a period of 1 ms after the end of the frame containing the
	//		HLTA command, this response shall be interpreted as 'not acknowledge'.
	// We interpret that this way: Only mfrc522_STATUS_TIMEOUT is an success.
	result = PCD_TransceiveData(buffer, sizeof(buffer), NULL_PTR, 0, NULL_PTR, 0, FALSE);
	if (result == mfrc522_STATUS_TIMEOUT) {
		return mfrc522_STATUS_OK;
	}
	if (result == mfrc522_STATUS_OK) { // That is ironically NOT ok in this case ;-)
		return mfrc522_STATUS_ERROR;
	}
	return result;
} // End PICC_HaltA()


/////////////////////////////////////////////////////////////////////////////////////
// Functions for communicating with MIFARE PICCs
/////////////////////////////////////////////////////////////////////////////////////

/**
 * Executes the MFRC522 MFAuthent command.
 * This command manages MIFARE authentication to enable a secure communication to any MIFARE Mini, MIFARE 1K and MIFARE 4K card.
 * The authentication is described in the MFRC522 datasheet section 10.3.1.9 and http://www.nxp.com/documents/data_sheet/MF1S503x.pdf section 10.1.
 * For use with MIFARE Classic PICCs.
 * The PICC must be selected - ie in state ACTIVE(*) - before calling this function.
 * Remember to call PCD_StopCrypto1() after communicating with the authenticated PICC - otherwise no new communications can start.
 *
 * All keys are set to FFFFFFFFFFFFh at chip delivery. A key consists of 6 bytes.
 *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise. Probably mfrc522_STATUS_TIMEOUT if you supply the wrong key.
 */
u8 PCD_Authenticate(u8 command,		///< PICC_CMD_MF_AUTH_KEY_A or PICC_CMD_MF_AUTH_KEY_B
								u8 blockAddr, 	///< The block number. See numbering in the comments in the .h file.
								MIFARE_Key *key,	///< Pointer to the Crypteo1 key to use (6 bytes)
								Uid *uid			///< Pointer to Uid struct. The first 4 bytes of the UID is used.
								) {
	u8 waitIRq = 0x10;		// IdleIRq

	// Build command buffer
	u8 sendData[12];
	sendData[0] = command;
	sendData[1] = blockAddr;
	for (u8 i = 0; i < MF_KEY_SIZE; i++) {	// 6 key bytes
		sendData[2+i] = key->keyByte[i];
	}
	for (u8 i = 0; i < 4; i++) {				// The first 4 bytes of the UID
		sendData[8+i] = uid->uidByte[i];
	}

	// Start the authentication.
	return PCD_CommunicateWithPICC(PCD_MFAuthent, waitIRq, &sendData[0], sizeof(sendData), NULL_PTR, 0, 0, 0, FALSE);
} // End PCD_Authenticate()

/**
 * Used to exit the PCD from its authenticated state.
 * Remember to call this function after communicating with an authenticated PICC - otherwise no new communications can start.
 */
void PCD_StopCrypto1() {
	// Clear MFCrypto1On bit
	PCD_ClearRegisterBitMask(Status2Reg, 0x08); // Status2Reg[7..0] bits are: TempSensClear I2CForceHS reserved reserved   MFCrypto1On ModemState[2:0]
} // End PCD_StopCrypto1()

/**
 * Reads 16 bytes (+ 2 bytes CRC_A) from the active PICC.
 *
 * For MIFARE Classic the sector containing the block must be authenticated before calling this function.
 *
 * For MIFARE Ultralight only addresses 00h to 0Fh are decoded.
 * The MF0ICU1 returns a NAK for higher addresses.
 * The MF0ICU1 responds to the READ command by sending 16 bytes starting from the page address defined by the command argument.
 * For example; if blockAddr is 03h then pages 03h, 04h, 05h, 06h are returned.
 * A roll-back is implemented: If blockAddr is 0Eh, then the contents of pages 0Eh, 0Fh, 00h and 01h are returned.
 *
 * The buffer must be at least 18 bytes because a CRC_A is also returned.
 * Checks the CRC_A before returning mfrc522_STATUS_OK.
 *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise.
 */
u8 MIFARE_Read(	u8 blockAddr, 	///< MIFARE Classic: The block (0-0xff) number. MIFARE Ultralight: The first page to return data from.
							u8 *buffer,		///< The buffer to store the data in
							u8 bufferSize	///< Buffer size, at least 18 bytes. Also number of bytes returned if mfrc522_STATUS_OK.
						) {
	u8 result;

	// Sanity check
	if (buffer == NULL_PTR || bufferSize < 18) {
		return mfrc522_STATUS_NO_ROOM;
	}

	// Build command buffer
	buffer[0] = PICC_CMD_MF_READ;
	buffer[1] = blockAddr;
	// Calculate CRC_A
	result = PCD_CalculateCRC(buffer, 2, &buffer[2]);
	if (result != mfrc522_STATUS_OK) {
		return result;
	}

	// Transmit the buffer and receive the response, validate CRC_A.
	return PCD_TransceiveData(buffer, 4, buffer, &bufferSize, NULL_PTR, 0, TRUE);
} // End MIFARE_Read()

/**
 * Writes 16 bytes to the active PICC.
 *
 * For MIFARE Classic the sector containing the block must be authenticated before calling this function.
 *
 * For MIFARE Ultralight the opretaion is called "COMPATIBILITY WRITE".
 * Even though 16 bytes are transferred to the Ultralight PICC, only the least significant 4 bytes (bytes 0 to 3)
 * are written to the specified address. It is recommended to set the remaining bytes 04h to 0Fh to all logic 0.
 * *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise.
 */
u8 MIFARE_Write(	u8 blockAddr, ///< MIFARE Classic: The block (0-0xff) number. MIFARE Ultralight: The page (2-15) to write to.
							u8 *buffer,	///< The 16 bytes to write to the PICC
							u8 bufferSize	///< Buffer size, must be at least 16 bytes. Exactly 16 bytes are written.
						) {
	u8 result;

	// Sanity check
	if (buffer == NULL_PTR || bufferSize < 16) {
		return mfrc522_STATUS_INVALID;
	}

	// Mifare Classic protocol requires two communications to perform a write.
	// Step 1: Tell the PICC we want to write to block blockAddr.
	u8 cmdBuffer[2];
	cmdBuffer[0] = PICC_CMD_MF_WRITE;
	cmdBuffer[1] = blockAddr;
	result = PCD_MIFARE_Transceive(cmdBuffer, 2, TRUE); // Adds CRC_A and checks that the response is MF_ACK.
	if (result != mfrc522_STATUS_OK) {
		return result;
	}

	// Step 2: Transfer the data
	result = PCD_MIFARE_Transceive(	buffer, bufferSize, TRUE); // Adds CRC_A and checks that the response is MF_ACK.
	if (result != mfrc522_STATUS_OK) {
		return result;
	}

	return mfrc522_STATUS_OK;
} // End MIFARE_Write()

/**
 * Writes a 4 u8 page to the active MIFARE Ultralight PICC.
 *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise.
 */
u8 MIFARE_Ultralight_Write(	u8 page, 		///< The page (2-15) to write to.
										u8 *buffer,	///< The 4 bytes to write to the PICC
										u8 bufferSize	///< Buffer size, must be at least 4 bytes. Exactly 4 bytes are written.
									) {
	u8 result;

	// Sanity check
	if (buffer == NULL_PTR || bufferSize < 4) {
		return mfrc522_STATUS_INVALID;
	}

	// Build commmand buffer
	u8 cmdBuffer[6];
	cmdBuffer[0] = PICC_CMD_UL_WRITE;
	cmdBuffer[1] = page;
	cmdBuffer[2] = buffer[0];
	cmdBuffer[3] = buffer[1];
	cmdBuffer[4] = buffer[2];
	cmdBuffer[5] = buffer[3];
	// memcpy(&cmdBuffer[2], buffer, 4);

	// Perform the write
	result = PCD_MIFARE_Transceive(cmdBuffer, 6, TRUE); // Adds CRC_A and checks that the response is MF_ACK.
	if (result != mfrc522_STATUS_OK) {
		return result;
	}
	return mfrc522_STATUS_OK;
} // End MIFARE_Ultralight_Write()

/**
 * MIFARE Decrement subtracts the delta from the value of the addressed block, and stores the result in a volatile memory.
 * For MIFARE Classic only. The sector containing the block must be authenticated before calling this function.
 * Only for blocks in "value block" mode, ie with access bits [C1 C2 C3] = [110] or [001].
 * Use MIFARE_Transfer() to store the result in a block.
 *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise.
 */
u8 MIFARE_Decrement(	u8 blockAddr, ///< The block (0-0xff) number.
								long delta		///< This number is subtracted from the value of block blockAddr.
							) {
	return MIFARE_TwoStepHelper(PICC_CMD_MF_DECREMENT, blockAddr, delta);
} // End MIFARE_Decrement()

/**
 * MIFARE Increment adds the delta to the value of the addressed block, and stores the result in a volatile memory.
 * For MIFARE Classic only. The sector containing the block must be authenticated before calling this function.
 * Only for blocks in "value block" mode, ie with access bits [C1 C2 C3] = [110] or [001].
 * Use MIFARE_Transfer() to store the result in a block.
 *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise.
 */
u8 MIFARE_Increment(	u8 blockAddr, ///< The block (0-0xff) number.
								long delta		///< This number is added to the value of block blockAddr.
							) {
	return MIFARE_TwoStepHelper(PICC_CMD_MF_INCREMENT, blockAddr, delta);
} // End MIFARE_Increment()

/**
 * MIFARE Restore copies the value of the addressed block into a volatile memory.
 * For MIFARE Classic only. The sector containing the block must be authenticated before calling this function.
 * Only for blocks in "value block" mode, ie with access bits [C1 C2 C3] = [110] or [001].
 * Use MIFARE_Transfer() to store the result in a block.
 *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise.
 */
u8 MIFARE_Restore(	u8 blockAddr ///< The block (0-0xff) number.
							) {
	// The datasheet describes Restore as a two step operation, but does not explain what data to transfer in step 2.
	// Doing only a single step does not work, so I chose to transfer 0L in step two.
	return MIFARE_TwoStepHelper(PICC_CMD_MF_RESTORE, blockAddr, 0L);
} // End MIFARE_Restore()

/**
 * MIFARE Transfer writes the value stored in the volatile memory into one MIFARE Classic block.
 * For MIFARE Classic only. The sector containing the block must be authenticated before calling this function.
 * Only for blocks in "value block" mode, ie with access bits [C1 C2 C3] = [110] or [001].
 *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise.
 */
u8 MIFARE_Transfer(	u8 blockAddr ///< The block (0-0xff) number.
							) {
	u8 result;
	u8 cmdBuffer[2]; // We only need room for 2 bytes.

	// Tell the PICC we want to transfer the result into block blockAddr.
	cmdBuffer[0] = PICC_CMD_MF_TRANSFER;
	cmdBuffer[1] = blockAddr;
	result = PCD_MIFARE_Transceive(	cmdBuffer, 2, TRUE); // Adds CRC_A and checks that the response is MF_ACK.
	if (result != mfrc522_STATUS_OK) {
		return result;
	}
	return mfrc522_STATUS_OK;
} // End MIFARE_Transfer()


/////////////////////////////////////////////////////////////////////////////////////
// Support functions
/////////////////////////////////////////////////////////////////////////////////////

/**
 * Wrapper for MIFARE protocol communication.
 * Adds CRC_A, executes the Transceive command and checks that the response is MF_ACK or a timeout.
 *
 * @return mfrc522_STATUS_OK on success, mfrc522_STATUS_??? otherwise.
 */
u8 PCD_MIFARE_Transceive(	u8 *sendData,		///< Pointer to the data to transfer to the FIFO. Do NOT include the CRC_A.
										u8 sendLen,		///< Number of bytes in sendData.
										boolean acceptTimeout	///< True => A timeout is also success
									) {
	u8 result;
	u8 cmdBuffer[18]; // We need room for 16 bytes data and 2 bytes CRC_A.
	u8 send_idx = 0 ;
	// Sanity check
	if (sendData == NULL_PTR || sendLen > 16) {
		return mfrc522_STATUS_INVALID;
	}

	// Copy sendData[] to cmdBuffer[] and add CRC_A
	// memcpy(cmdBuffer, sendData, sendLen);
	for(send_idx=0 ; send_idx<sendLen ; send_idx++)
	{
		cmdBuffer[send_idx] = sendData[send_idx];
	}
	
	result = PCD_CalculateCRC(cmdBuffer, sendLen, &cmdBuffer[sendLen]);
	if (result != mfrc522_STATUS_OK) {
		return result;
	}
	sendLen += 2;

	// Transceive the data, store the reply in cmdBuffer[]
	u8 waitIRq = 0x30;		// RxIRq and IdleIRq
	u8 cmdBufferSize = sizeof(cmdBuffer);
	u8 validBits = 0;
	result = PCD_CommunicateWithPICC(PCD_Transceive, waitIRq, cmdBuffer, sendLen, cmdBuffer, &cmdBufferSize, &validBits, 0, FALSE);
	if (acceptTimeout && result == mfrc522_STATUS_TIMEOUT) {
		return mfrc522_STATUS_OK;
	}
	if (result != mfrc522_STATUS_OK) {
		return result;
	}
	// The PICC must reply with a 4 bit ACK
	if (cmdBufferSize != 1 || validBits != 4) {
		return mfrc522_STATUS_ERROR;
	}
	if (cmdBuffer[0] != MF_ACK) {
		return mfrc522_STATUS_MIFARE_NACK;
	}
	return mfrc522_STATUS_OK;
} // End PCD_MIFARE_Transceive()

/**
 * Returns a string pointer to a status code name.
 *
 */
const char *GetStatusCodeName(u8 code	///< One of the StatusCode enums.
										) {
	switch (code) {
		case mfrc522_STATUS_OK:				return "Success."; break;
		case mfrc522_STATUS_ERROR:			return "Error in communication."; break;
		case mfrc522_STATUS_COLLISION:		return "Collission detected."; break;
		case mfrc522_STATUS_TIMEOUT:		return "Timeout in communication."; break;
		case mfrc522_STATUS_NO_ROOM:		return "A buffer is not big enough."; break;
		case mfrc522_STATUS_INTERNAL_ERROR:	return "Internal error in the code. Should not happen."; break;
		case mfrc522_STATUS_INVALID:		return "Invalid argument."; break;
		case mfrc522_STATUS_CRC_WRONG:		return "The CRC_A does not match."; break;
		case mfrc522_STATUS_MIFARE_NACK:	return "A MIFARE PICC responded with NAK."; break;
		default:
			return "Unknown error";
			break;
	}
} // End GetStatusCodeName()

/**
 * Translates the SAK (Select Acknowledge) to a PICC type.
 *
 * @return PICC_Type
 */
u8 PICC_GetType(u8 sak		///< The SAK u8 returned from PICC_Select().
							) {
	if (sak & 0x04) { // UID not complete
		return PICC_TYPE_NOT_COMPLETE;
	}

	switch (sak) {
		case 0x09:	return PICC_TYPE_MIFARE_MINI;	break;
		case 0x08:	return PICC_TYPE_MIFARE_1K;		break;
		case 0x18:	return PICC_TYPE_MIFARE_4K;		break;
		case 0x00:	return PICC_TYPE_MIFARE_UL;		break;
		case 0x10:
		case 0x11:	return PICC_TYPE_MIFARE_PLUS;	break;
		case 0x01:	return PICC_TYPE_TNP3XXX;		break;
		default:	break;
	}

	if (sak & 0x20) {
		return PICC_TYPE_ISO_14443_4;
	}

	if (sak & 0x40) {
		return PICC_TYPE_ISO_18092;
	}

	return PICC_TYPE_UNKNOWN;
} // End PICC_GetType()

u8 PCD_DumbVersion(void)
{
	u8 v = PCD_ReadRegister(VersionReg);
	return v; 
}
/**
 * Returns a string pointer to the PICC type name.
 *
 */
const char *PICC_GetTypeName(u8 piccType	///< One of the PICC_Type enums.
										) {
	switch (piccType) {
		case PICC_TYPE_ISO_14443_4:		return "PICC compliant with ISO/IEC 14443-4";		break;
		case PICC_TYPE_ISO_18092:		return "PICC compliant with ISO/IEC 18092 (NFC)";	break;
		case PICC_TYPE_MIFARE_MINI:		return "MIFARE Mini, 320 bytes";					break;
		case PICC_TYPE_MIFARE_1K:		return "MIFARE 1KB";								break;
		case PICC_TYPE_MIFARE_4K:		return "MIFARE 4KB";								break;
		case PICC_TYPE_MIFARE_UL:		return "MIFARE Ultralight or Ultralight C";			break;
		case PICC_TYPE_MIFARE_PLUS:		return "MIFARE Plus";								break;
		case PICC_TYPE_TNP3XXX:			return "MIFARE TNP3XXX";							break;
		case PICC_TYPE_NOT_COMPLETE:	return "SAK indicates UID is not complete.";		break;
		case PICC_TYPE_UNKNOWN:
		default:						return "Unknown type";								break;
	}
} // End PICC_GetTypeName()

#ifdef mfrc522_DumpToSerial
/**
 * Dumps debug info about the selected PICC to Serial.
 * On success the PICC is halted after dumping the data.
 * For MIFARE Classic the factory default key of 0xFFFFFFFFFFFF is tried.
 */
void PICC_DumpToSerial(Uid *uid	///< Pointer to Uid struct returned from a successful PICC_Select().
								) {
	MIFARE_Key key;

	// UID
	printf("Card UID:");
	for (u8 i = 0; i < uid->size; i++) {
		printf("%s", uid->uidByte[i] < 0x10 ? " 0" : " ");
		printf("%02x", uid->uidByte[i]);
	}
	printf("\r\n");

	// PICC type
	u8 piccType = PICC_GetType(uid->sak);
	printf("PICC type: %s\r\n", PICC_GetTypeName(piccType));

	// Dump contents
	switch (piccType) {
		case PICC_TYPE_MIFARE_MINI:
		case PICC_TYPE_MIFARE_1K:
		case PICC_TYPE_MIFARE_4K:
			// All keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
			for (u8 i = 0; i < 6; i++) {
				key.keyByte[i] = 0xFF;
			}
			PICC_DumpMifareClassicToSerial(uid, piccType, &key);
			break;

		case PICC_TYPE_MIFARE_UL:
			PICC_DumpMifareUltralightToSerial();
			break;

		case PICC_TYPE_ISO_14443_4:
		case PICC_TYPE_ISO_18092:
		case PICC_TYPE_MIFARE_PLUS:
		case PICC_TYPE_TNP3XXX:
			printf("Dumping memory contents not implemented for that PICC type.");
			break;

		case PICC_TYPE_UNKNOWN:
		case PICC_TYPE_NOT_COMPLETE:
		default:
			break; // No memory dump here
	}

	printf("\r\n");
	PICC_HaltA(); // Already done if it was a MIFARE Classic PICC.
} // End PICC_DumpToSerial()

#endif

#ifdef mfrc522_DumpToSerial
/**
 * Dumps memory contents of a MIFARE Classic PICC.
 * On success the PICC is halted after dumping the data.
 */
void PICC_DumpMifareClassicToSerial(	Uid *uid,		///< Pointer to Uid struct returned from a successful PICC_Select().
												u8 piccType,	///< One of the PICC_Type enums.
												MIFARE_Key *key	///< Key A used for all sectors.
											) {
	u8 no_of_sectors = 0;
	switch (piccType) {
		case PICC_TYPE_MIFARE_MINI:
			// Has 5 sectors * 4 blocks/sector * 16 bytes/block = 320 bytes.
			no_of_sectors = 5;
			break;

		case PICC_TYPE_MIFARE_1K:
			// Has 16 sectors * 4 blocks/sector * 16 bytes/block = 1024 bytes.
			no_of_sectors = 16;
			break;

		case PICC_TYPE_MIFARE_4K:
			// Has (32 sectors * 4 blocks/sector + 8 sectors * 16 blocks/sector) * 16 bytes/block = 4096 bytes.
			no_of_sectors = 40;
			break;

		default: // Should not happen. Ignore.
			break;
	}

	// Dump sectors, highest address first.
	if (no_of_sectors) {
		printf("Sector Block   0  1  2  3   4  5  6  7   8  9 10 11  12 13 14 15  AccessBits\r\n");
		for (char i = no_of_sectors - 1; i >= 0; i--) {
			PICC_DumpMifareClassicSectorToSerial(uid, key, i);
		}
	}
	PICC_HaltA(); // Halt the PICC before stopping the encrypted session.
	PCD_StopCrypto1();
} // End PICC_DumpMifareClassicToSerial()
#endif

#ifdef mfrc522_DumpToSerial
/**
 * Dumps memory contents of a sector of a MIFARE Classic PICC.
 * Uses PCD_Authenticate(), MIFARE_Read() and PCD_StopCrypto1.
 * Always uses PICC_CMD_MF_AUTH_KEY_A because only Key A can always read the sector trailer access bits.
 */
void PICC_DumpMifareClassicSectorToSerial(Uid *uid,			///< Pointer to Uid struct returned from a successful PICC_Select().
													MIFARE_Key *key,	///< Key A for the sector.
													u8 sector			///< The sector to dump, 0..39.
													) {
	u8 status;
	u8 firstBlock;		// Address of lowest address to dump actually last block dumped)
	u8 no_of_blocks;		// Number of blocks in sector
	boolean isSectorTrailer;	// Set to TRUE while handling the "last" (ie highest address) in the sector.

	// The access bits are stored in a peculiar fashion.
	// There are four groups:
	//		g[3]	Access bits for the sector trailer, block 3 (for sectors 0-31) or block 15 (for sectors 32-39)
	//		g[2]	Access bits for block 2 (for sectors 0-31) or blocks 10-14 (for sectors 32-39)
	//		g[1]	Access bits for block 1 (for sectors 0-31) or blocks 5-9 (for sectors 32-39)
	//		g[0]	Access bits for block 0 (for sectors 0-31) or blocks 0-4 (for sectors 32-39)
	// Each group has access bits [C1 C2 C3]. In this code C1 is MSB and C3 is LSB.
	// The four CX bits are stored together in a nible cx and an inverted nible cx_.
	u8 c1, c2, c3;		// Nibbles
	u8 c1_, c2_, c3_;		// Inverted nibbles
	boolean invertedError;		// True if one of the inverted nibbles did not match
	u8 g[4];				// Access bits for each of the four groups.
	u8 group;				// 0-3 - active group for access bits
	boolean firstInGroup;		// True for the first block dumped in the group

	// Determine position and size of sector.
	if (sector < 32) { // Sectors 0..31 has 4 blocks each
		no_of_blocks = 4;
		firstBlock = sector * no_of_blocks;
	}
	else if (sector < 40) { // Sectors 32-39 has 16 blocks each
		no_of_blocks = 16;
		firstBlock = 128 + (sector - 32) * no_of_blocks;
	}
	else { // Illegal input, no MIFARE Classic PICC has more than 40 sectors.
		return;
	}

	// Dump blocks, highest address first.
	u8 byteCount;
	u8 buffer[18];
	u8 blockAddr;
	isSectorTrailer = TRUE;
	for (char blockOffset = no_of_blocks - 1; blockOffset >= 0; blockOffset--) {
		blockAddr = firstBlock + blockOffset;
		// Sector number - only on first line
		if (isSectorTrailer) {
			printf("%s%d   ", sector < 10 ? "   " : "  ", sector); // Pad with spaces
		}
		else {
			printf("       ");
		}
		// Block number
		printf("%s%d  ", blockAddr < 10 ? "   " : (blockAddr < 100 ? "  "	 : " "), blockAddr); // Pad with spaces
		// Establish encrypted communications before reading the first block
		if (isSectorTrailer) {
			status = PCD_Authenticate(PICC_CMD_MF_AUTH_KEY_A, firstBlock, key, uid);
			if (status != mfrc522_STATUS_OK) {
				printf("PCD_Authenticate() failed: %s\r\n", GetStatusCodeName(status));
				return;
			}
		}
		// Read block
		byteCount = sizeof(buffer);
		status = MIFARE_Read(blockAddr, buffer, &byteCount);
		if (status != mfrc522_STATUS_OK) {
			printf("MIFARE_Read() failed: %s\r\n", GetStatusCodeName(status));
			continue;
		}
		// Dump data
		for (u8 index = 0; index < 16; index++) {
			printf(" %02x", buffer[index]);
			if ((index % 4) == 3) {
				printf(" ");
			}
		}
		// Parse sector trailer data
		if (isSectorTrailer) {
			c1  = buffer[7] >> 4;
			c2  = buffer[8] & 0xF;
			c3  = buffer[8] >> 4;
			c1_ = buffer[6] & 0xF;
			c2_ = buffer[6] >> 4;
			c3_ = buffer[7] & 0xF;
			invertedError = (c1 != (~c1_ & 0xF)) || (c2 != (~c2_ & 0xF)) || (c3 != (~c3_ & 0xF));
			g[0] = ((c1 & 1) << 2) | ((c2 & 1) << 1) | ((c3 & 1) << 0);
			g[1] = ((c1 & 2) << 1) | ((c2 & 2) << 0) | ((c3 & 2) >> 1);
			g[2] = ((c1 & 4) << 0) | ((c2 & 4) >> 1) | ((c3 & 4) >> 2);
			g[3] = ((c1 & 8) >> 1) | ((c2 & 8) >> 2) | ((c3 & 8) >> 3);
			isSectorTrailer = FALSE;
		}

		// Which access group is this block in?
		if (no_of_blocks == 4) {
			group = blockOffset;
			firstInGroup = TRUE;
		}
		else {
			group = blockOffset / 5;
			firstInGroup = (group == 3) || (group != (blockOffset + 1) / 5);
		}

		if (firstInGroup) {
			// Print access bits
			printf(" [ %d %d %d ] ", (g[group] >> 2) & 1, (g[group] >> 1) & 1, (g[group] >> 0) & 1);
			if (invertedError) {
				printf(" Inverted access bits did not match! ");
			}
		}

		if (group != 3 && (g[group] == 1 || g[group] == 6)) { // Not a sector trailer, a value block
			long value = ((long)(buffer[3])<<24) | ((long)(buffer[2])<<16) | ((long)(buffer[1])<<8) | (long)(buffer[0]);
			printf(" Value=0x%02x Adr=0x%02x", value, buffer[12]);
		}
		printf("\r\n");
	}

	return;
} // End PICC_DumpMifareClassicSectorToSerial()

#endif
#ifdef mfrc522_DumpToSerial
/**
 * Dumps memory contents of a MIFARE Ultralight PICC.
 */
void PICC_DumpMifareUltralightToSerial() {
	u8 status;
	u8 byteCount;
	u8 buffer[18];
	u8 i;

	printf("Page  0  1  2  3\r\n");
	// Try the mpages of the original Ultralight. Ultralight C has more pages.
	for (u8 page = 0; page < 16; page +=4) { // Read returns data for 4 pages at a time.
		// Read pages
		byteCount = sizeof(buffer);
		status = MIFARE_Read(page, buffer, &byteCount);
		if (status != mfrc522_STATUS_OK) {
			printf("MIFARE_Read() failed: %s", GetStatusCodeName(status));
			break;
		}
		// Dump data
		for (u8 offset = 0; offset < 4; offset++) {
			i = page + offset;
			printf("%s", i < 10 ? "  " : " "); // Pad with spaces
			printf("%d", i);
			printf("  ");
			for (u8 index = 0; index < 4; index++) {
				i = 4 * offset + index;
				printf("%s", buffer[i] < 0x10 ? " 0" : " ");
				printf("%02x", buffer[i]);
			}
			printf("\r\n");
		}
	}
} // End PICC_DumpMifareUltralightToSerial()
#endif

/**
 * Calculates the bit pattern needed for the specified access bits. In the [C1 C2 C3] tupples C1 is MSB (=4) and C3 is LSB (=1).
 */
void MIFARE_SetAccessBits(	u8 *accessBitBuffer,	///< Pointer to u8 6, 7 and 8 in the sector trailer. Bytes [0..2] will be set.
									u8 g0,				///< Access bits [C1 C2 C3] for block 0 (for sectors 0-31) or blocks 0-4 (for sectors 32-39)
									u8 g1,				///< Access bits C1 C2 C3] for block 1 (for sectors 0-31) or blocks 5-9 (for sectors 32-39)
									u8 g2,				///< Access bits C1 C2 C3] for block 2 (for sectors 0-31) or blocks 10-14 (for sectors 32-39)
									u8 g3					///< Access bits C1 C2 C3] for the sector trailer, block 3 (for sectors 0-31) or block 15 (for sectors 32-39)
								) {
	u8 c1 = (u8)((g3 & 4) << 1) | (u8)((g2 & 4) << 0) | (u8)((g1 & 4) >> 1) | (u8)((g0 & 4) >> 2);
	u8 c2 = (u8)((g3 & 2) << 2) | (u8)((g2 & 2) << 1) | (u8)((g1 & 2) << 0) | (u8)((g0 & 2) >> 1);
	u8 c3 = (u8)((g3 & 1) << 3) | (u8)((g2 & 1) << 2) | (u8)((g1 & 1) << 1) | (u8)((g0 & 1) << 0);

	accessBitBuffer[0] = (u8)((~c2 & 0xF) << 4 )| (~c1 & 0xF);
	accessBitBuffer[1] = (u8)(c1 << 4) | (~c3 & 0xF);
	accessBitBuffer[2] = (u8)(c3 << 4) | c2;
} // End MIFARE_SetAccessBits()

/////////////////////////////////////////////////////////////////////////////////////
// Convenience functions - does not add extra functionality
/////////////////////////////////////////////////////////////////////////////////////

/**
 * Returns TRUE if a PICC responds to PICC_CMD_REQA.
 * Only "new" cards in state IDLE are invited. Sleeping cards in state HALT are ignored.
 *
 * @return boolean
 */
boolean PICC_IsNewCardPresent() 
{
	u8 bufferATQA[2];
	u8 bufferSize = sizeof(bufferATQA);
	u8 result = PICC_RequestA(bufferATQA, &bufferSize);
	return (result == mfrc522_STATUS_OK || result == mfrc522_STATUS_COLLISION);
} // End PICC_IsNewCardPresent()

/**
 * Simple wrapper around PICC_Select.
 * Returns TRUE if a UID could be read.
 * Remember to call PICC_IsNewCardPresent(), PICC_RequestA() or PICC_WakeupA() first.
 * The read UID is available in the class variable uid.
 *
 * @return boolean
 */
boolean PICC_ReadCardSerial() {
	u8 result = PICC_Select(&uid, 0);
	return (result == mfrc522_STATUS_OK);//return a '1' if PICC_Select returns a '1', else a '0'
} // End PICC_ReadCardSerial()

int PICC_WriteBlock(int blockNumber, u8 arrayAddress[], MIFARE_Key *key )
{
  //this makes sure that we only write into data blocks. Every 4th block is a trailer block for the access/security info.
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;//determine trailer block for the sector
  if (blockNumber > 2 && (blockNumber+1)%4 == 0){
	//  printf("%d is a trailer block:", blockNumber);
	  return 2;
  }//block number is a trailer block (modulo 4); quit and send error code 2
  //printf("%d is a data block:", blockNumber);

  /*****************************************authentication of the desired block for access***********************************************************/
  u8 status = PCD_Authenticate(PICC_CMD_MF_AUTH_KEY_A, trailerBlock, key , &uid);
  //u8 PCD_Authenticate(u8 command, u8 blockAddr, MIFARE_Key *key, Uid *uid);
  //this method is used to authenticate a certain block for writing or reading
  //command: See enumerations above -> PICC_CMD_MF_AUTH_KEY_A	= 0x60 (=1100000),		// this command performs authentication with Key A
  //blockAddr is the number of the block from 0 to 15.
  //MIFARE_Key *key is a pointer to the MIFARE_Key struct defined above, this struct needs to be defined for each block. New cards have all A/B= FF FF FF FF FF FF
  //Uid *uid is a pointer to the UID struct that contains the user ID of the card.
  if (status != mfrc522_STATUS_OK) {
//         printf("PCD_Authenticate() failed: %s\r\n", GetStatusCodeName(status));
         return 3;//return "3" as error message
  }
  //it appears the authentication needs to be made before every block read/write within a specific sector.
  //If a different sector is being authenticated access to the previous one is lost.

  /*****************************************writing the block***********************************************************/

  status = MIFARE_Write(blockNumber, arrayAddress, 16);//valueBlockA is the block number, MIFARE_Write(block number (0-15), u8 array containing 16 values, number of bytes in block (=16))
  //status = mfrc522.MIFARE_Write(9, value1Block, 16);
  if (status != mfrc522_STATUS_OK) {
    //       printf("MIFARE_Write() failed: %s\r\n", GetStatusCodeName(status));
           return 4;//return "4" as error message
  }
  //printf("block was written\r\n");
  return status;
}


int PICC_ReadBlock(int blockNumber, u8 arrayAddress[], MIFARE_Key *key )
{
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;//determine trailer block for the sector

  /*****************************************authentication of the desired block for access***********************************************************/
  u8 status = PCD_Authenticate(PICC_CMD_MF_AUTH_KEY_A, trailerBlock, key, &(uid));
  //u8 PCD_Authenticate(u8 command, u8 blockAddr, MIFARE_Key *key, Uid *uid);
  //this method is used to authenticate a certain block for writing or reading
  //command: See enumerations above -> PICC_CMD_MF_AUTH_KEY_A	= 0x60 (=1100000),		// this command performs authentication with Key A
  //blockAddr is the number of the block from 0 to 15.
  //MIFARE_Key *key is a pointer to the MIFARE_Key struct defined above, this struct needs to be defined for each block. New cards have all A/B= FF FF FF FF FF FF
  //Uid *uid is a pointer to the UID struct that contains the user ID of the card.
  if (status != mfrc522_STATUS_OK) {
 //        printf("PCD_Authenticate() failed (read): %s\r\n", GetStatusCodeName(status));
         return 3;//return "3" as error message
  }
  //it appears the authentication needs to be made before every block read/write within a specific sector.
  //If a different sector is being authenticated access to the previous one is lost.


  /*****************************************reading a block***********************************************************/

  u8 buffersize = 18;//we need to define a variable with the read buffer size, since the MIFARE_Read method below needs a pointer to the variable that contains the size...
  status = MIFARE_Read(blockNumber, arrayAddress, &buffersize);//&buffersize is a pointer to the buffersize variable; MIFARE_Read requires a pointer instead of just a number
  if (status != mfrc522_STATUS_OK) {
 //         printf("MIFARE_read() failed: %s\r\n", GetStatusCodeName(status));
          return 4;//return "4" as error message
  }
  //printf("block was read\r\n");
  return status;
}

Uid PCD_getUid(void)
{
	return uid; 
}
