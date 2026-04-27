.CODE
_Clear_board_asm PROC
    push rdi
    mov rdi, rcx
    mov rcx, rdx
    xor rax, rax
    rep stosq
    pop rdi
    ret
_Clear_board_asm ENDP

_Reset_ball PROC
    mov eax, dword ptr [rcx + 32]
    shr eax, 1
    mov dword ptr [rcx + 0], eax
    mov eax, dword ptr [rcx + 36]
    shr eax, 1
    mov dword ptr [rcx + 4], eax
    mov eax, dword ptr [rcx + 8]
    neg eax
    mov dword ptr [rcx + 8], eax
    ret
_Reset_ball ENDP

_Update_pong_physics_asm PROC
    mov eax, dword ptr [rcx + 0]
    add eax, dword ptr [rcx + 8]
    mov dword ptr [rcx + 0], eax
    mov r8d, dword ptr [rcx + 4]
    add r8d, dword ptr [rcx + 12]
    mov dword ptr [rcx + 4], r8d
    cmp r8d, 0
    jle _flip_vy
    mov r9d, dword ptr [rcx + 36]
    dec r9d
    cmp r8d, r9d
    jl _skip_flip_vy
_flip_vy:
    mov r9d, dword ptr [rcx + 12]
    neg r9d
    mov dword ptr [rcx + 12], r9d
_skip_flip_vy:
    mov r9d, dword ptr [rcx + 20]
    cmp r8d, r9d
    jge _ai_down
    cmp r9d, 0
    jle _ai_done
    dec r9d
    mov dword ptr [rcx + 20], r9d
    jmp _ai_done
_ai_down:
    mov r10d, r9d
    add r10d, dword ptr [rcx + 40]
    cmp r8d, r10d
    jle _ai_done
    mov r11d, dword ptr [rcx + 36]
    sub r11d, dword ptr [rcx + 40]
    cmp r9d, r11d
    jge _ai_done
    inc r9d
    mov dword ptr [rcx + 20], r9d
_ai_done:
    cmp eax, 1
    jg _right_paddle
    mov r9d, dword ptr [rcx + 16]
    cmp r8d, r9d
    jl _left_miss
    mov r10d, r9d
    add r10d, dword ptr [rcx + 40]
    cmp r8d, r10d
    jge _left_miss
    mov r11d, dword ptr [rcx + 8]
    neg r11d
    mov dword ptr [rcx + 8], r11d
    mov dword ptr [rcx + 0], 2
    jmp _paddle_done
_left_miss:
    cmp eax, 0
    jge _paddle_done
    mov r11d, dword ptr [rcx + 28]
    inc r11d
    mov dword ptr [rcx + 28], r11d
    call _Reset_ball
    jmp _paddle_done
_right_paddle:
    mov r10d, dword ptr [rcx + 32]
    sub r10d, 2
    cmp eax, r10d
    jl _paddle_done
    mov r9d, dword ptr [rcx + 20]
    cmp r8d, r9d
    jl _right_miss
    mov r11d, r9d
    add r11d, dword ptr [rcx + 40]
    cmp r8d, r11d
    jge _right_miss
    mov r11d, dword ptr [rcx + 8]
    neg r11d
    mov dword ptr [rcx + 8], r11d
    mov r11d, dword ptr [rcx + 32]
    sub r11d, 3
    mov dword ptr [rcx + 0], r11d
    jmp _paddle_done
_right_miss:
    mov r11d, dword ptr [rcx + 32]
    cmp eax, r11d
    jl _paddle_done
    mov r11d, dword ptr [rcx + 24]
    inc r11d
    mov dword ptr [rcx + 24], r11d
    call _Reset_ball
_paddle_done:
    ret
_Update_pong_physics_asm ENDP
END