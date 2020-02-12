
kernel:     file format elf64-x86-64


Disassembly of section .text:

ffffffff80100000 <main>:
ffffffff80100000:	f3 0f 1e fa          	endbr64 
ffffffff80100004:	55                   	push   %rbp
ffffffff80100005:	48 89 e5             	mov    %rsp,%rbp
ffffffff80100008:	41 b8 ff 00 00 00    	mov    $0xff,%r8d
ffffffff8010000e:	b9 ff 00 00 00       	mov    $0xff,%ecx
ffffffff80100013:	ba ff 00 00 00       	mov    $0xff,%edx
ffffffff80100018:	be 00 04 00 00       	mov    $0x400,%esi
ffffffff8010001d:	bf 58 02 00 00       	mov    $0x258,%edi
ffffffff80100022:	e8 11 01 00 00       	callq  ffffffff80100138 <_Z9draw_linejjhhh>
ffffffff80100027:	eb fe                	jmp    ffffffff80100027 <main+0x27>

ffffffff80100029 <_Z14is_pixel_validii>:
ffffffff80100029:	f3 0f 1e fa          	endbr64 
ffffffff8010002d:	55                   	push   %rbp
ffffffff8010002e:	48 89 e5             	mov    %rsp,%rbp
ffffffff80100031:	89 7d ec             	mov    %edi,-0x14(%rbp)
ffffffff80100034:	89 75 e8             	mov    %esi,-0x18(%rbp)
ffffffff80100037:	48 c7 45 f8 00 e0 00 	movq   $0xe000,-0x8(%rbp)
ffffffff8010003e:	00 
ffffffff8010003f:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
ffffffff80100043:	0f b7 40 02          	movzwl 0x2(%rax),%eax
ffffffff80100047:	0f b7 c0             	movzwl %ax,%eax
ffffffff8010004a:	89 45 f4             	mov    %eax,-0xc(%rbp)
ffffffff8010004d:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
ffffffff80100051:	0f b7 40 04          	movzwl 0x4(%rax),%eax
ffffffff80100055:	0f b7 c0             	movzwl %ax,%eax
ffffffff80100058:	89 45 f0             	mov    %eax,-0x10(%rbp)
ffffffff8010005b:	83 7d ec 00          	cmpl   $0x0,-0x14(%rbp)
ffffffff8010005f:	78 16                	js     ffffffff80100077 <_Z14is_pixel_validii+0x4e>
ffffffff80100061:	83 7d e8 00          	cmpl   $0x0,-0x18(%rbp)
ffffffff80100065:	78 10                	js     ffffffff80100077 <_Z14is_pixel_validii+0x4e>
ffffffff80100067:	8b 45 ec             	mov    -0x14(%rbp),%eax
ffffffff8010006a:	39 45 f4             	cmp    %eax,-0xc(%rbp)
ffffffff8010006d:	76 08                	jbe    ffffffff80100077 <_Z14is_pixel_validii+0x4e>
ffffffff8010006f:	8b 45 e8             	mov    -0x18(%rbp),%eax
ffffffff80100072:	39 45 f0             	cmp    %eax,-0x10(%rbp)
ffffffff80100075:	77 02                	ja     ffffffff80100079 <_Z14is_pixel_validii+0x50>
ffffffff80100077:	eb fe                	jmp    ffffffff80100077 <_Z14is_pixel_validii+0x4e>
ffffffff80100079:	b8 01 00 00 00       	mov    $0x1,%eax
ffffffff8010007e:	5d                   	pop    %rbp
ffffffff8010007f:	c3                   	retq   

ffffffff80100080 <_Z9set_pixeliihhh>:
ffffffff80100080:	f3 0f 1e fa          	endbr64 
ffffffff80100084:	55                   	push   %rbp
ffffffff80100085:	48 89 e5             	mov    %rsp,%rbp
ffffffff80100088:	48 83 ec 38          	sub    $0x38,%rsp
ffffffff8010008c:	89 7d dc             	mov    %edi,-0x24(%rbp)
ffffffff8010008f:	89 75 d8             	mov    %esi,-0x28(%rbp)
ffffffff80100092:	89 c8                	mov    %ecx,%eax
ffffffff80100094:	44 89 c1             	mov    %r8d,%ecx
ffffffff80100097:	88 55 d4             	mov    %dl,-0x2c(%rbp)
ffffffff8010009a:	88 45 d0             	mov    %al,-0x30(%rbp)
ffffffff8010009d:	89 c8                	mov    %ecx,%eax
ffffffff8010009f:	88 45 cc             	mov    %al,-0x34(%rbp)
ffffffff801000a2:	48 c7 45 f8 00 e0 00 	movq   $0xe000,-0x8(%rbp)
ffffffff801000a9:	00 
ffffffff801000aa:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
ffffffff801000ae:	48 8b 40 08          	mov    0x8(%rax),%rax
ffffffff801000b2:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
ffffffff801000b6:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
ffffffff801000ba:	0f b7 40 02          	movzwl 0x2(%rax),%eax
ffffffff801000be:	0f b7 c0             	movzwl %ax,%eax
ffffffff801000c1:	89 45 ec             	mov    %eax,-0x14(%rbp)
ffffffff801000c4:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
ffffffff801000c8:	0f b6 40 06          	movzbl 0x6(%rax),%eax
ffffffff801000cc:	c0 e8 03             	shr    $0x3,%al
ffffffff801000cf:	0f b6 c0             	movzbl %al,%eax
ffffffff801000d2:	89 45 e8             	mov    %eax,-0x18(%rbp)
ffffffff801000d5:	48 c7 45 e0 00 00 00 	movq   $0x0,-0x20(%rbp)
ffffffff801000dc:	00 
ffffffff801000dd:	8b 55 d8             	mov    -0x28(%rbp),%edx
ffffffff801000e0:	8b 45 dc             	mov    -0x24(%rbp),%eax
ffffffff801000e3:	89 d6                	mov    %edx,%esi
ffffffff801000e5:	89 c7                	mov    %eax,%edi
ffffffff801000e7:	e8 3d ff ff ff       	callq  ffffffff80100029 <_Z14is_pixel_validii>
ffffffff801000ec:	84 c0                	test   %al,%al
ffffffff801000ee:	74 45                	je     ffffffff80100135 <_Z9set_pixeliihhh+0xb5>
ffffffff801000f0:	8b 45 d8             	mov    -0x28(%rbp),%eax
ffffffff801000f3:	0f af 45 ec          	imul   -0x14(%rbp),%eax
ffffffff801000f7:	89 c2                	mov    %eax,%edx
ffffffff801000f9:	8b 45 dc             	mov    -0x24(%rbp),%eax
ffffffff801000fc:	01 d0                	add    %edx,%eax
ffffffff801000fe:	0f af 45 e8          	imul   -0x18(%rbp),%eax
ffffffff80100102:	89 c2                	mov    %eax,%edx
ffffffff80100104:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
ffffffff80100108:	48 01 d0             	add    %rdx,%rax
ffffffff8010010b:	48 89 45 e0          	mov    %rax,-0x20(%rbp)
ffffffff8010010f:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
ffffffff80100113:	0f b6 55 cc          	movzbl -0x34(%rbp),%edx
ffffffff80100117:	88 10                	mov    %dl,(%rax)
ffffffff80100119:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
ffffffff8010011d:	48 8d 50 01          	lea    0x1(%rax),%rdx
ffffffff80100121:	0f b6 45 d0          	movzbl -0x30(%rbp),%eax
ffffffff80100125:	88 02                	mov    %al,(%rdx)
ffffffff80100127:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
ffffffff8010012b:	48 8d 50 02          	lea    0x2(%rax),%rdx
ffffffff8010012f:	0f b6 45 d4          	movzbl -0x2c(%rbp),%eax
ffffffff80100133:	88 02                	mov    %al,(%rdx)
ffffffff80100135:	90                   	nop
ffffffff80100136:	c9                   	leaveq 
ffffffff80100137:	c3                   	retq   

ffffffff80100138 <_Z9draw_linejjhhh>:
ffffffff80100138:	f3 0f 1e fa          	endbr64 
ffffffff8010013c:	55                   	push   %rbp
ffffffff8010013d:	48 89 e5             	mov    %rsp,%rbp
ffffffff80100140:	48 83 ec 38          	sub    $0x38,%rsp
ffffffff80100144:	89 7d dc             	mov    %edi,-0x24(%rbp)
ffffffff80100147:	89 75 d8             	mov    %esi,-0x28(%rbp)
ffffffff8010014a:	89 c8                	mov    %ecx,%eax
ffffffff8010014c:	44 89 c1             	mov    %r8d,%ecx
ffffffff8010014f:	88 55 d4             	mov    %dl,-0x2c(%rbp)
ffffffff80100152:	88 45 d0             	mov    %al,-0x30(%rbp)
ffffffff80100155:	89 c8                	mov    %ecx,%eax
ffffffff80100157:	88 45 cc             	mov    %al,-0x34(%rbp)
ffffffff8010015a:	48 c7 45 f0 00 e0 00 	movq   $0xe000,-0x10(%rbp)
ffffffff80100161:	00 
ffffffff80100162:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
ffffffff80100166:	0f b7 40 02          	movzwl 0x2(%rax),%eax
ffffffff8010016a:	0f b7 c0             	movzwl %ax,%eax
ffffffff8010016d:	89 45 ec             	mov    %eax,-0x14(%rbp)
ffffffff80100170:	8b 45 ec             	mov    -0x14(%rbp),%eax
ffffffff80100173:	2b 45 d8             	sub    -0x28(%rbp),%eax
ffffffff80100176:	d1 e8                	shr    %eax
ffffffff80100178:	89 45 fc             	mov    %eax,-0x4(%rbp)
ffffffff8010017b:	8b 55 ec             	mov    -0x14(%rbp),%edx
ffffffff8010017e:	8b 45 d8             	mov    -0x28(%rbp),%eax
ffffffff80100181:	01 d0                	add    %edx,%eax
ffffffff80100183:	d1 e8                	shr    %eax
ffffffff80100185:	89 c2                	mov    %eax,%edx
ffffffff80100187:	8b 45 fc             	mov    -0x4(%rbp),%eax
ffffffff8010018a:	39 c2                	cmp    %eax,%edx
ffffffff8010018c:	76 22                	jbe    ffffffff801001b0 <_Z9draw_linejjhhh+0x78>
ffffffff8010018e:	0f b6 7d cc          	movzbl -0x34(%rbp),%edi
ffffffff80100192:	0f b6 4d d0          	movzbl -0x30(%rbp),%ecx
ffffffff80100196:	0f b6 55 d4          	movzbl -0x2c(%rbp),%edx
ffffffff8010019a:	8b 75 dc             	mov    -0x24(%rbp),%esi
ffffffff8010019d:	8b 45 fc             	mov    -0x4(%rbp),%eax
ffffffff801001a0:	41 89 f8             	mov    %edi,%r8d
ffffffff801001a3:	89 c7                	mov    %eax,%edi
ffffffff801001a5:	e8 d6 fe ff ff       	callq  ffffffff80100080 <_Z9set_pixeliihhh>
ffffffff801001aa:	83 45 fc 01          	addl   $0x1,-0x4(%rbp)
ffffffff801001ae:	eb cb                	jmp    ffffffff8010017b <_Z9draw_linejjhhh+0x43>
ffffffff801001b0:	90                   	nop
ffffffff801001b1:	c9                   	leaveq 
ffffffff801001b2:	c3                   	retq   
