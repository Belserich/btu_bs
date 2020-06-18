#include "interrupts/IntLock.h"
#include "thread/ActivityScheduler.h" // erbt von queue
#include "thread/Activity.h"

#ifndef BoundedBuffer_h
#define BoundedBuffer_h


/** Diese Klasse stellt einen begrenzten synchronisierten
 *  Puffer dar, welcher von Interruptbehandlungsroutinen
 *  gef�llt werden kann.
 *
 *  Die Klasse ist eine Templateklasse. Das bedeutet f�r euch
 *  erstmal nur, das ihr alle Methoden hier im Header implementieren
 *  m�sst.
 */
template<typename T,unsigned size>
class BoundedBuffer {
public:

    /** Diese Methode wird vom Interrupthandler aufgerufen, um
     *  ein etwas in den Puffer zu schreiben. Ist der Puffer voll,
     *  werden die zu schreibenden Daten verworfen.
     *  Prozesse die auf eine Eingabe warten m�ssen hier geweckt werden.
     */
    void add(T& elem)
    {
    	if (index < size)
		{
			buffer[index++] = elem;
		}

    	for (Chain* co = queue.dequeue(); co != nullptr; co = queue.dequeue())
		{
			((Activity*) co)->wakeup();
		}
//        if (first != last) {
//			buffer[last] = elem;
//			this->last = (last + 1) % size;
//			if (queue.getHead()) {
//				((Activity*) queue.getHead())->wakeup();
//			}
//		}
	}

	/** Diese Methode wird von Prozessen aufgerufen, um Daten aus dem
	 *  Puffer zu lesen. Ist dieser leer wird der lesende Prozess schlafen
	 *  gelegt. Achtet hier besonders auf die Synchronisierung.
	 */
    T get()
    {
    	IntLock lock;
    	while (index == 0)
		{
    		Activity* act = (Activity*) scheduler.active();
    		queue.enqueue((Chain*) act);
    		CPU::enableInterrupts();
    		act->sleep();
    		CPU::disableInterrupts();
		}

		return buffer[--index];
//        Activity* temp = (Activity*)scheduler.active();
//
//        if(temp != queue.getHead())
//        {
//            queue.enqueue(temp);
//            temp->sleep();
//        }
//
//        if (last == (first+1)%size)
//        {
//            temp->sleep();
//        }
//
//        queue.dequeue();
//        this->first = (first+1) % size;
//        return buffer[first];
    }

private:
    T buffer[size];

    unsigned int index = 0;
    unsigned int first = size - 1;
    unsigned int last = 0;

    Queue queue;
};

#endif
