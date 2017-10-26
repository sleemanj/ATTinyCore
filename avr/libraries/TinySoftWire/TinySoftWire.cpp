#include "TinySoftWire.h"

TinySoftWire::TinySoftWire(void)
{
  
}

void TinySoftWire::begin(void)  
{
  #if WIRE_RX_BUFFER_LENGTH > 0
    rxBufferIndex = 0;
    rxBufferLength = 0;
  #endif
  
  error = 0;
  transmitting = false;

  i2c_init();
}

void TinySoftWire::end(void) 
{ 

}

void TinySoftWire::setClock(uint32_t _) 
{
  
}

void TinySoftWire::beginTransmission(uint8_t address) 
{
  if (transmitting) {
    error = (i2c_rep_start((address<<1)|I2C_WRITE) ? 0 : 2);
  } else {
    error = (i2c_start((address<<1)|I2C_WRITE) ? 0 : 2);
  }
  // indicate that we are transmitting
  transmitting = 1;
}

void TinySoftWire::beginTransmission(int address) 
{
  beginTransmission((uint8_t)address);
}

uint8_t TinySoftWire::endTransmission(uint8_t sendStop)
{
  uint8_t transError = error;
  if (sendStop) {
    i2c_stop();
    transmitting = 0;
  }
  error = 0;
  return transError;
}

uint8_t TinySoftWire::endTransmission(void)
{
  return endTransmission(true);
}

size_t TinySoftWire::write(uint8_t data) 
{
  if (i2c_write(data)) {
    return 1;
  } else {
    if (error == 0) error = 3;
    return 0;
  }
}

size_t TinySoftWire::write(const uint8_t *data, size_t quantity) 
{
  size_t trans = 0;
  for(size_t i = 0; i < quantity; ++i){
    trans += write(data[i]);
  }
  return trans;
}

#if WIRE_RX_BUFFER_LENGTH > 0

uint8_t TinySoftWire::requestFrom(uint8_t address, uint8_t quantity,
          uint32_t iaddress, uint8_t isize, uint8_t sendStop) 
{
  uint8_t localerror = 0;
  if (isize > 0) {
    // send internal address; this mode allows sending a repeated start to access
    // some devices' internal registers. This function is executed by the hardware
    // TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)
    beginTransmission(address);
    // the maximum size of internal address is 3 bytes
    if (isize > 3){
isize = 3;
    }
    // write internal register address - most significant byte first
    while (isize-- > 0)
write((uint8_t)(iaddress >> (isize*8)));
    endTransmission(false);
  }
  // clamp to buffer length
  if(quantity > WIRE_RX_BUFFER_LENGTH){
    quantity = WIRE_RX_BUFFER_LENGTH;
  }
  localerror = !i2c_rep_start((address<<1) | I2C_READ);
  if (error == 0 && localerror) error = 2;
  // perform blocking read into buffer
  for (uint8_t cnt=0; cnt < quantity; cnt++) 
    rxBuffer[cnt] = i2c_read(cnt == quantity-1);
  // set rx buffer iterator vars
  rxBufferIndex = 0;
  rxBufferLength = quantity;
  if (sendStop) {
    transmitting = 0;
    i2c_stop();
  }
  return quantity;
}

uint8_t TinySoftWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) 
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
}

uint8_t  TinySoftWire::requestFrom(int address, int quantity, int sendStop) 
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}


uint8_t  TinySoftWire::requestFrom(uint8_t address, uint8_t quantity) 
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t  TinySoftWire::requestFrom(int address, int quantity) 
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

int TinySoftWire::available(void) 
{
  return rxBufferLength - rxBufferIndex;
}

int TinySoftWire::read(void) 
{
  int value = -1;
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }
  return value;
}

int TinySoftWire::peek(void) 
{
  int value = -1;
  
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
  }
  return value;
}

#endif

TinySoftWire Wire;