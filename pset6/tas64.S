	.text				#See tas32.S (32-bit version) for
.globl tas				#other comments
	.type	tas,@function
tas:
	movq	$1, %rax
	lock;xchgb	%al,(%rdi)	#arg1 is in the rdi register
	movsbq	%al,%rax		#sign-extend result into rax
	ret				#rax contains the return value
.Lfe1:
	.size	tas,.Lfe1-tas

    