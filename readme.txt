Usage: RunFunge.exe <file> [-bf93]

-bf93 deactivates all extensions

Befunge-93 Commands:

0-9	Pushes this number on the stack
+	Pops B and A, then pushes A + B on the stack
-	Pops B and A, then pushes A - B on the stack
*	Pops B and A, then pushes A * B on the stack
/	Pops B and A, then pushes floor(A / B) on the stack
%	Pops B and A, then pushes A % B on the stack (Modulo)
!	Pops a value, pushes 1 if it is 0, else 0
`	Pops B and A, pushes 1 if A < B, else 0
>	Changes direction to right
<	Changes direction to left
^	Changes direction to up
v	Changes direction to down
?	Changes direction by random
_	Pops a value, changes direction to right if 0, else left
|	Pops a value, changes direction to down if 0, else up
"	Starts/ends stringmode
:	Dublicates first value on stack
\	Swaps the top two values on stack
$	Discard first value on stack
.	Pops a value and outputs it as a number
,	Pops a value and outputs it as an ASCII character
#	Skip next cell
g	Pops Y and X and pushes the ASCII of the command at X/Y within the code
p	Pops Y, X and V and changes the command at X/Y within the code to v
&	Prompts for a number and pushes it on the stack
~	Prompts for a ASCII character and pushes its ASCII code on the stack
@	Quits program

Extended Commands:

A-F	Pushes 10 through 15
m	Move To: Pops Y and X and moves the IP to X/Y keeping direction
	(next step is interpretation of the command at X/Y)
u	Conditional Move Up: Pops a value, if it is 0, behave like ^, otherwise keep direction
d	Conditional Move Down: Pops a value, if it is 0, behave like v, otherwise keep direction
l	Conditional Move Left: Pops a value, if it is 0, behave like <, otherwise keep direction
r	Conditional Move Right: Pops a value, if it is 0, behave like >, otherwise keep direction
[	Turn Left: Turn direction of IP left by 90 degree (borrowed from Funge-98)
]	Turn Right: Turn direction of IP right by 90 degree (borrowed from Funge-98)
t	Turn Around: Move to opposite direction
w	Compare: Pops B and A, acts like [ if A < B, acts like ] if A > B, keep direction if a = b (borrowed from Funge-98)
c	Clear: Clears the whole stack
§	Constant Random: When IP reaches a §, it is randomly replaced by either <, >, ^, or v
S	Stack Size: Pushes the number of elements in the stack (not counting the new one it pushes)
x	Save Coordinates: Puts current X and Y position on the stack
{	Call Sub Routine: Pops Y and X and moves IP to X/Y keeping direction
	(next step is interpretation of the command at X/Y)
}	Return: Restores position and direction of last {-call
=	Execute: Pops a complete string, executes it as a system command and pushes its return value on the stack
@	Quit With Return Value: Pops a value and quits program returning that value (0 = no error)