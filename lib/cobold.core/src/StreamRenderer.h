#pragma once
#include <Arduino.h>
#include "MemoryStream.h"
#include <freertos/semphr.h>

namespace cobold::sys
{

    class StreamRenderer
    {
    private:
        MemoryStream *stream;
        SemaphoreHandle_t mutex;

        void printFormat(const char format, va_list *args)
        {
            if (format == '\0')
                return;
            if (format == '%')
            {
                stream->print(format);
            }
            else if (format == 's')
            {
                register char *s = va_arg(*args, char *);
                stream->print(s);
            }
            else if (format == 'S')
            {
                register __FlashStringHelper *s = va_arg(*args, __FlashStringHelper *);
                stream->print(s);
            }
            else if (format == 'd' || format == 'i')
            {
                stream->print(va_arg(*args, int), DEC);
            }
            else if (format == 'D' || format == 'F')
            {
                stream->print(va_arg(*args, double));
            }
            else if (format == 'x')
            {
                stream->print(va_arg(*args, int), HEX);
            }
            else if (format == 'X')
            {
                stream->print("0x");
                //_logOutput->print(va_arg(*args, int), HEX);
                register uint16_t h = (uint16_t)va_arg(*args, int);
                if (h < 0xFFF)
                    stream->print('0');
                if (h < 0xFF)
                    stream->print('0');
                if (h < 0xF)
                    stream->print('0');
                stream->print(h, HEX);
            }
            else if (format == 'p')
            {
                register Printable *obj = (Printable *)va_arg(*args, int);
                stream->print(*obj);
            }
            else if (format == 'b')
            {
                stream->print(va_arg(*args, int), BIN);
            }
            else if (format == 'B')
            {
                stream->print("0b");
                stream->print(va_arg(*args, int), BIN);
            }
            else if (format == 'l')
            {
                stream->print(va_arg(*args, long), DEC);
            }
            else if (format == 'u')
            {
                stream->print(va_arg(*args, unsigned long), DEC);
            }
            else if (format == 'c')
            {
                stream->print((char)va_arg(*args, int));
            }
            else if (format == 'C')
            {
                register char c = (char)va_arg(*args, int);
                if (c >= 0x20 && c < 0x7F)
                {
                    stream->print(c);
                }
                else
                {
                    stream->print("0x");
                    if (c < 0xF)
                        stream->print('0');
                    stream->print(c, HEX);
                }
            }
            else if (format == 't')
            {
                if (va_arg(*args, int) == 1)
                {
                    stream->print("T");
                }
                else
                {
                    stream->print("F");
                }
            }
            else if (format == 'T')
            {
                if (va_arg(*args, int) == 1)
                {
                    stream->print(F("true"));
                }
                else
                {
                    stream->print(F("false"));
                }
            }
        }

        void print(const char *format, va_list args)
        {
            // This copy is only necessary on some architectures (x86) to change a passed
            // array in to a va_list.

            for (; *format != 0; ++format)
            {
                if (*format == '%')
                {
                    ++format;

                    printFormat(*format, &args);
                }
                else
                {
                    stream->print(*format);
                }
            }
        }

        void print(const __FlashStringHelper *format, va_list args)
        {
            PGM_P p = reinterpret_cast<PGM_P>(format);
            // This copy is only necessary on some architectures (x86) to change a passed
            // array in to a va_list.

            char c = pgm_read_byte(p++);
            for (; c != 0; c = pgm_read_byte(p++))
            {
                if (c == '%')
                {
                    c = pgm_read_byte(p++);

                    printFormat(c, &args);
                }
                else
                {
                    stream->print(c);
                }
            }
        }

        void print(const Printable &obj, va_list args)
        {
            stream->print(obj);
        }

    public:
        StreamRenderer()
        {
            this->stream = new MemoryStream();
            this->mutex = xSemaphoreCreateMutex();
        }

        template <class T>
        const char *render(T msg, ...)
        {
            xSemaphoreTake(mutex, portMAX_DELAY);

            va_list args;
            va_start(args, msg);
            print(msg, args);
            va_end(args);
            const char *result = (new std::string(reinterpret_cast<char *>(stream->buffer)))->c_str();
            stream->clear();

            xSemaphoreGive(mutex);
            return result;
        }
    };

} // namespace cobold::sys