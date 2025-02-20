#include <xinu.h>

/*
 *COSC 3250 - Project 3
 * Implements Synchronous Serial Driver
 * @author Justin Hoffman & Luke Bondi
 * Instructor Dr.Brylow
 *TA-BOT:MAILTO justin.hoffman@marquette.edu
 */

#define UNGETMAX 10             /* Can un-get at most 10 characters. */
static unsigned char ungetArray[UNGETMAX];
unsigned int  bufp;

syscall kgetc()
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    // TODO: First, check the unget buffer for a character.
    //       Otherwise, check UART line status register, and
    //       once there is data ready, get character c.
    
    if (bufp > 0) {
	    return ungetArray[--bufp];
    }
    while (!(regptr->lsr & UART_LSR_DR)) {}
    return regptr->rbr;

    return SYSERR;
}

/**
 * kcheckc - check to see if a character is available.
 * @return true if a character is available, false otherwise.
 */
syscall kcheckc(void)
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    // TODO: Check the unget buffer and the UART for characters.

    if (bufp != 0) {
	return 1;
    } else if (regptr->lsr & UART_LSR_DR) {
	return 1;
    } else {
	return 0;
    }

	return SYSERR;
}

/**
 * kungetc - put a serial character "back" into a local buffer.
 * @param c character to unget.
 * @return c on success, SYSERR on failure.
 */
syscall kungetc(unsigned char c)
{
    // TODO: Check for room in unget buffer, put the character in or discard.

    if (bufp == UNGETMAX) {
	    return SYSERR;
    } else {
	ungetArray[bufp] = c;
	bufp++;
	return c;
    }

    return SYSERR;
}

syscall kputc(uchar c)
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    // TODO: Check UART line status register.
    //       Once the Transmitter FIFO is empty, send character c.

    while (!(regptr->lsr & UART_LSR_THRE)) {}
    regptr->thr = c;

    return c;
}

syscall kprintf(const char *format, ...)
{
    int retval;
    va_list ap;

    va_start(ap, format);
    retval = _doprnt(format, ap, (int (*)(long, long))kputc, 0);
    va_end(ap);
    return retval;
}
