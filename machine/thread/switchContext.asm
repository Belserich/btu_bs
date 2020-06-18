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
;     void switchContext (void*& from, void*& to);
;

switchContext:
	;Auf den Stack speichern
    push    ebp
    mov     ebp, esp
    push    ebx
    push    esi
    push    edi
	;Adressen holen
    mov     eax, [ebp+8]
    mov     [eax], esp
    mov     eax, [ebp+12]
    mov     esp, [eax]   
	;Vom Stack holen
    pop     edi
    pop     esi
    pop     ebx
    pop     ebp

	ret		; Ruecksprung zum Aufrufer

