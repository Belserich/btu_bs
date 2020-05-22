%include "../machine/csym.inc"	; Innerhalb des Include-Files 'csym.inc'
				; wird das Macro CSYM(name) definiert,
				; das dem uebergebenen Symbolnamen in
				; Abhaegigkeit vom verwendeten Generierungs-
				; system einen Unterstrich ('_') voranstellt.

; EXPORTIERTE FUNKTIONEN

csym switchContext

[GLOBAL switchContext]

; IMPLEMENTIERUNG DER FUNKTIONEN

[SECTION .text]

; switchContext: Der Coroutinenwechsel
;
; C-Prototyp:
;
;     void switchContext (void* &from, void* &to);
;

switchContext:

    push    ebp
    mov     ebp, esp

    push    esi
    push    edi
    push    ebx

    mov     eax, [ebp+8]        ; eax = from (pointer)
    mov     [eax], esp          ; *eax = esp (pointer) alter esp an from-Adresse gespeichert
    mov     eax, [ebp+12]       ; eax = to (pointer)
    mov     esp, [eax]          ; esp = *eax

; 1#switchContext(...)
; Co1 -> Co2
; alten Stackpointer speichern
; eax = Co1.from (=Co1.sp)
; *eax = esp (also *Co1.sp = esp)
; esp auf den neuen pointer setzen
; eax = Co2.to
; esp = *eax (= Co2.to)

; 2#switchContext(...)
; Co2 -> Co1
; alten Stackpointer speichern
; eax = Co2.from
; *eax = esp
; esp auf den neuen pointer setzen
; eax = Co1.to (=Co1.sp)
; esp = *eax (=Co1.to (=Co1.sp)) (also: esp = *Co1.sp)

    pop     ebx
    pop     edi
    pop     esi

    pop     ebp

	ret		; Ruecksprung zum Aufrufer

