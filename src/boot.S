#
# guzhoudiaoke@126.com
# 2020-01-20
#

.section .text
.global _start
.code16

_start:
	movw	%cx,		%ax
	movw	%ax,		%ds
	movw	%ax,		%es

	call	clear_screen
	call    draw_message

1:
	jmp		1b

clear_screen:
	movb	$0x06,		%ah
	movb	$0,			%al
	movb	$0,			%ch
	movb	$0,			%ch
	movb	$24,		%dh
	movb	$79,		%dl
	movb	$0x07,		%bh
	int		$0x10
	ret

draw_message:
	movw	$msg,		%bp
	movw	len,		%cx
	movb	$0x02,		%dh
	movb	$0x02,		%dl
	movb	$0x0c,		%bl
	movb	$0x00,		%bh

	movb	$0x01,		%al
	movb	$0x13,		%ah
	int		$0x10

msg:
	.asciz	"Hello babyos!"
len:
	.int	. - msg

.org  0x1fe, 0x90
.word 0xaa55
