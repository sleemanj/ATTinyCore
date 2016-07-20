static inline void _tone(const uint8_t pin, const uint32_t frequency, const uint32_t length)
{
  uint16_t  prescaleDivider = 1; // The divider increases by powers of 2
  uint8_t   prescaleBitMask = 1; // The bitmask only increments unitarily
  uint32_t  ticksPerMs      = F_CPU/1000;
  switch(F_CPU)
  {
    case 9600000UL:
    {
      if(frequency >= 37648) 
      {
        prescaleDivider = 1;
        ticksPerMs      = F_CPU/1000;
        prescaleBitMask = 1;
      }
      else if(frequency >= 4706)
      {
        prescaleDivider = 8;
        ticksPerMs      = (F_CPU/8)/1000;
        prescaleBitMask = 2;
      }
      else if(frequency >= 589)
      {
        prescaleDivider = 64;
        ticksPerMs      = (F_CPU/64)/1000;
        prescaleBitMask = 3;
      }
      else if(frequency >= 148)
      {
        prescaleDivider = 256;
        ticksPerMs      = (F_CPU/256)/1000;
        prescaleBitMask = 4;
      }
      else if(frequency >= 37)
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