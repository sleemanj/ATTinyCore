static inline void _tone(const uint8_t pin, const uint32_t frequency, const uint32_t length)
{
  uint16_t  prescaleDivider = 1; // The divider increases by powers of 2
  uint8_t   prescaleBitMask = 1; // The bitmask only increments unitarily
  uint32_t  ticksPerMs      = F_CPU/1000;
  switch(F_CPU)
  {
    case 9600000UL:
    {
      if(frequency >= 37648)     // (9600000UL / 1) / 255
      {
        prescaleDivider = 1;
        ticksPerMs      = F_CPU/1000;
        prescaleBitMask = 1;
      }
      else if(frequency >= 4706)  // (9600000UL / 8) / 255
      {
        prescaleDivider = 8;
        ticksPerMs      = (F_CPU/8)/1000;
        prescaleBitMask = 2;
      }
      else if(frequency >= 589)  // (9600000UL / 64) / 255
      {
        prescaleDivider = 64;
        ticksPerMs      = (F_CPU/64)/1000;
        prescaleBitMask = 3;
      }
      else if(frequency >= 148)  // (9600000UL / 256) / 255
      {
        prescaleDivider = 256;
        ticksPerMs      = (F_CPU/256)/1000;
        prescaleBitMask = 4;
      }
      else if(frequency >= 37)  // (9600000UL / 1024) / 255
      {
        prescaleDivider = 1024;
        ticksPerMs      = (F_CPU/1024)/1000;
        prescaleBitMask = 5;
      }
    }
    break;
    
    case 4800000UL:
    {
      if(frequency >= 18824) 
      {
        prescaleDivider = 1;
        ticksPerMs      = F_CPU/1000;
        prescaleBitMask = 1;
      }
      else if(frequency >= 2353)
      {
        prescaleDivider = 8;
        ticksPerMs      = (F_CPU/8)/1000;
        prescaleBitMask = 2;
      }
      else if(frequency >= 295)
      {
        prescaleDivider = 64;
        ticksPerMs      = (F_CPU/64)/1000;
        prescaleBitMask = 3;
      }
      else if(frequency >= 74)
      {
        prescaleDivider = 256;
        ticksPerMs      = (F_CPU/256)/1000;
        prescaleBitMask = 4;
      }
      else if(frequency >= 19)
      {
        prescaleDivider = 1024;
        ticksPerMs      = (F_CPU/1024)/1000;
        prescaleBitMask = 5;
      }
    }
    break;
    
    case 1200000UL:
    {
      if(frequency >= 4706) 
      {
        prescaleDivider = 1;
        ticksPerMs      = F_CPU/1000;
        prescaleBitMask = 1;
      }
      else if(frequency >= 589)
      {
        prescaleDivider = 8;
        ticksPerMs      = (F_CPU/8)/1000;
        prescaleBitMask = 2;
      }
      else if(frequency >= 74)
      {
        prescaleDivider = 64;
        ticksPerMs      = (F_CPU/64)/1000;
        prescaleBitMask = 3;
      }
      else if(frequency >= 74)
      {
        prescaleDivider = 19;
        ticksPerMs      = (F_CPU/256)/1000;
        prescaleBitMask = 4;
      }
      else if(frequency >= 5)
      {
        prescaleDivider = 1024;
        ticksPerMs      = (F_CPU/1024)/1000;
        prescaleBitMask = 5;
      }
    }
    break;
    
    default:
      return;
  }
  
  _toneRaw(pin, ((F_CPU / prescaleDivider) / frequency), length ? length * ticksPerMs : ~(0UL), prescaleBitMask);
}