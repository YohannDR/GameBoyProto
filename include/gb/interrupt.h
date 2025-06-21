#ifndef GB_INTERRUPT_H
#define GB_INTERRUPT_H

#define INTR_VBLANK (1 << 0)
#define INTR_LCD    (1 << 1)
#define INTR_TIMER  (1 << 2)
#define INTR_SERIAL (1 << 3)

#define STAT_HBLANK (1 << 2)
#define STAT_MODE_0 (1 << 3)
#define STAT_MODE_1 (1 << 4)
#define STAT_MODE_2 (1 << 5)
#define STAT_LCY    (1 << 6)

#endif /* GB_INTERRUPT_H */
