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

    mov     eax, [ebp+8]       ; eax := from
    mov     [eax], esp          ; [eax] = [from] := esp
    mov     eax, [ebp+12]
    mov     esp, [eax]       ; esp := to

    pop     ebx
    pop     edi
    pop     esi

    pop     ebp

	ret		; Ruecksprung zum Aufrufer

