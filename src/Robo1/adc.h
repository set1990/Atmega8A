#ifndef ADC_H_
#define ADC_H_

#define ADC_MAX		5
#define ADC_VOID	4
#define ADC_NEXT	6

uint16_t ADC_value[ADC_MAX];
uint8_t ADC_fresh[ADC_MAX];

void ADC_Handle(void);

#endif /* ADC_H_ */
