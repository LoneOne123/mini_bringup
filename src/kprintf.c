/* Based on my ft_printf from the 42 curriculum, ported to bare metal (output via UART, no libc). */

#include "kprintf.h"
#include <stddef.h>
#include <stdarg.h>
#include "uart.h"

static size_t ft_strlen(const char *s){
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

static void pl_unsigned(unsigned long n, unsigned long base, const char *digits, int *count){
	if (n >= base){
		pl_unsigned(n/base, base, digits, count);
	}
	uart_putc(digits[n % base]);
	*count += 1;
}

static void pl_signed(long n, int *count){
	if (n < 0){
		uart_putc('-');
		*count += 1;
		pl_unsigned(0UL - (unsigned long) n, 10UL, "0123456789", count);
	}
	else
		pl_unsigned((unsigned long) n, 10UL, "0123456789", count);
}

static void pl_pointer(void *pointer, int *count){
	unsigned long	ptr;

	if (pointer == NULL){
        uart_puts("(nil)");
		*count += 5;
		return ;
	}
	uart_puts("0x");
    *count += 2;
	ptr = (unsigned long) pointer;
	pl_unsigned(ptr, 16UL, "0123456789abcdef", count);
}

static void pl_str(char *str, int *count){
	if (str == NULL){
        uart_puts("(null)");
		*count += 6;
		return ;
	}
	*count += ft_strlen(str);
    uart_puts(str);
}

static void pl_char(char c, int *count){
    uart_putc(c);
	*count += 1;
}

static int pl_wrong(const char *format, int *count, int *i){
	if (format[*i] == 0)
		return (0);
	else{
        uart_putc('%');
		*count += 1;
		*i -= 1;
	}
	return (1);
}

static int put_arg(const char *format, int *i, int *count, va_list *args){
	*i = *i + 1;
	int is_long = 0;
	if (format[*i] == 'l') {
		is_long = 1;
		*i = *i + 1;
	}
	if (format[*i] == 'c')
		pl_char(va_arg(*args, int), count);
	else if (format[*i] == 's')
		pl_str(va_arg(*args, char *), count);
	else if (format[*i] == 'p'){
		pl_pointer(va_arg(*args, void *), count);
	}
	else if (format[*i] == 'i' || format[*i] == 'd')
		if (!is_long)
			pl_signed(va_arg(*args, int), count);
		else
			pl_signed(va_arg(*args, long), count);
	else if (format[*i] == 'u')
		if (!is_long)
			pl_unsigned(va_arg(*args, unsigned int), 10UL, "0123456789", count);
		else
			pl_unsigned(va_arg(*args, unsigned long), 10UL, "0123456789", count);
	else if (format[*i] == 'x')
		if (!is_long)
			pl_unsigned(va_arg(*args, unsigned int), 16UL, "0123456789abcdef", count);
		else
			pl_unsigned(va_arg(*args, unsigned long), 16UL, "0123456789abcdef", count);
	else if (format[*i] == 'X')
		if (!is_long)
			pl_unsigned(va_arg(*args, unsigned int), 16UL, "0123456789ABCDEF", count);
		else
			pl_unsigned(va_arg(*args, unsigned long), 16UL, "0123456789ABCDEF", count);
	else if (format[*i] == '%'){
        uart_putc('%');
		*count += 1;
	}
	else if (pl_wrong(format, count, i) == 0)
		return (0);
	*i = *i + 1;
	return (1);
}

int	kprintf(const char *format, ...)
{
	int		i;
	va_list	args;
	int		count;

	i = 0;
	count = 0;
	if (format == NULL)
		return (-1);
	va_start(args, format);
	while (format[i]){
		if (format[i] != '%'){
            uart_putc(format[i]);
			i++;
			count++;
		}
		else{
			if (put_arg(format, &i, &count, &args) == 0){
                va_end(args);
				return (-1);
            }
		}
	}
	va_end(args);
	return (count);
}