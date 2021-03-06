#include "interrupts/SpuriousGate.h"
#include "interrupts/InterruptVector.h"
#include "device/PIC.h"

SpuriousGate::SpuriousGate(): Gate(SpuriousInterrupt) { }

void SpuriousGate::handle()
{
	PIC pic = PIC();
	pic.ack();
}
