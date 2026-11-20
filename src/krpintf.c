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

static void ft_putnbr(int n){
	long int	nb;
	char		c;

	nb = (long int) n;
	if (nb < 0){
		uart_putc('-');
        nb *= -1;
	}
	if (nb > 9){
        ft_putnbr(nb / 10);
    }
	c = (nb % 10) + '0';
    uart_putc(c);
}

static void ft_putunbr(unsigned int nb){
	if (nb > 9){
        ft_putunbr(nb / 10);
    }
    ft_putnbr(nb % 10);
}


static void pl_uint(unsigned int nb, int *count){
	ft_putunbr(nb);
	while (nb / 10 != 0){
		nb /= 10;
		*count += 1;
	}
	*count += 1;
}

static void pl_int(int nb, int *count){
	if (nb < 0)
		*count += 1;
	ft_putnbr(nb);
	while (nb / 10 != 0){
		nb /= 10;
		*count += 1;
	}
	*count += 1;
}

static void num_to_hex(unsigned long num, int *count){
	if (num > 15)
		num_to_hex(num / 16, count);
	uart_putc("0123456789abcdef"[num % 16]);
    *count += 1;
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
	num_to_hex(ptr, count);
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

static void nb_to_hex(unsigned int nb, int *count){
	if (nb > 15)
		nb_to_hex(nb / 16, count);
	uart_putc("0123456789abcdef"[nb % 16]);
    *count += 1;
}

static void pl_hex_low(unsigned int nb, int *count){
	nb_to_hex(nb, count);
}

static void nb_to_hex_up(unsigned int nb, int *count){
	if (nb > 15)
		nb_to_hex_up(nb / 16, count);
	uart_putc("0123456789ABCDEF"[nb % 16]);
    *count += 1;
}

static void pl_hex_up(unsigned int nb, int *count){
	nb_to_hex_up(nb, count);
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
	if (format[*i] == 'c')
		pl_char(va_arg(*args, int), count);
	else if (format[*i] == 's')
		pl_str(va_arg(*args, char *), count);
	else if (format[*i] == 'p')
		pl_pointer(va_arg(*args, void *), count);
	else if (format[*i] == 'i' || format[*i] == 'd')
		pl_int(va_arg(*args, int), count);
	else if (format[*i] == 'u')
		pl_uint(va_arg(*args, unsigned int), count);
	else if (format[*i] == 'x')
		pl_hex_low(va_arg(*args, unsigned int), count);
	else if (format[*i] == 'X')
		pl_hex_up(va_arg(*args, unsigned int), count);
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