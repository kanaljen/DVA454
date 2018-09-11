/* bitoperations.c
 *
 * Demonstration of bitwise operations.
 * Author: Marcus Jansson
 * Date: 2010-03-11
 *
 */

#define TRUE 1
#define FALSE 0

int main(void)
{
	unsigned char op1, op2;
	unsigned char result;

	/* Bitwise AND */
	op1 = 0x01;
	op2 = 0x03;
	result = op1 & op2; //0000 0011 AND 0000 0001 (1) -> 0000 0001 (1)

	/* Bitwise AND, a variant */
	/* This is standard C and works much like the short form of
	 * a += b;
	 * which means:
	 * a = a + b;
	 */
	op1 = 0x03;
	result = 0x07;
	result &= op1;	/* result = result & op2 */ // 0000 0011 (3) AND 0000 0111 (7) -> 0000 0011 (3)

	/* Bitwise OR */
	op1 = 0x01;
	op2 = 0x03;
	result = op1 | op2; //0000 0001 OR 0000 0011 (3) -> 0000 0011 (3)

	/* Bitwise XOR */
	op1 = 0x01;
	op2 = 0x03;
	result = op1 ^ op2; //0000 0001 XOR 0000 0011 (3) -> 0000 0010 (2)

	/* Bitwise NOT */
	op1 = 0x01;
	result = ~op1 ;  // 0000 0001 (1) -> 1111 1110 (254)

	/* Bitwise LEFT SHIFT */
	op1 = 0x02;
	result = (op1 >> 1); //0000 0010 (2) -> 0000 0001 (1)

	/* Bitwise LEFT SHIFT again */
	op1 = 0x80;
	result = (op1 >> 4); // 1000 0000 (128) -> 0000 1000 (8)

	/* Bitwise RIGHT SHIFT */
	op1 = 0x01;
	result = (op1 << 1); // 0000 0001 (1) -> 0000 0010 (2)

	/* Bitwise RIGHT SHIFT again */
	op1 = 0x01;
	result = (op1 << 7); // 0000 0001 (1) -> 0100 0000 (64)

	/* Logical Comparision AND */
	op1 = 0x01;
	op2 = 0x03;
	if(op1 && op2) {
		result = TRUE; /* true */
	}
	else {
		result = FALSE; /* not true */
	}

	/* Logical Comparision OR */
	op1 = 0x01;
	op2 = 0x03;
	if(op1 || op2) {
		result = TRUE; /* true */
	}
	else {
		result = FALSE; /* not true */
	}


	/* Do not let the main()-function finish and return.
	 * In embedded systems without OS there is usually nothing to return to.
	 */
	while(TRUE);
}