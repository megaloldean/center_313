#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

#include "center313.h"

/* Дескриптор устройства UART */
int dev = 0;

int center313_init(char *devName)
{
	/* Открываем устройство UART */
	dev = open(devName, O_RDWR | O_NOCTTY);
	if (0 == dev)
	{
		return -1;
	}
	else
	{
		/* Настраиваем UART */
		struct termios options;
		if (-1 == tcgetattr(dev, &options))
		{
			close(dev);
			dev = 0;
			return -1;
		}
		cfsetispeed(&options, B9600);
		cfsetospeed(&options, B9600);
		options.c_cflag &= ~CSIZE;
		options.c_cflag &= ~CSTOPB;
		options.c_cflag &= ~PARENB;
		options.c_cflag |= CS8;
		if (-1 == tcsetattr(dev, TCSANOW, &options))
		{
			close(dev);
			dev = 0;
			return -1;
		}
		return 0;
	}
}


void center313_close()
{
	if (0 != dev)
	{
		close(dev);
		dev = 0;
	}
}


int center313_getValues(float *temper, float *humidity)
{
	/* Буфер для отправки и чтения данных */
	unsigned char buf[RESPONSE_SIZE] = {0};
	buf[0] = REQUEST_CODE;
	/* Счетчик принятых байт */
	int buf_counter = 0;
	
	/* Отправляем запрос на получение значений */
	if (write(dev, buf, 1) < 0)
	{
		return -1;
	}
	
	/* Ожидает прием ответа от устройства */
	while (buf_counter < RESPONSE_SIZE)
	{
		buf_counter += read(dev, buf + buf_counter, RESPONSE_SIZE - buf_counter);
	}
	
	*humidity = (float)(((buf[3] << 8) + buf[4]) / 10.0f);
	*temper = (float)(((buf[5] << 8) + buf[6]) / 10.0f);
	return 0;
}
