/** Libraries */
#include <stdarg.h> // mandatory header for variadic functions
#include <unistd.h> // mandatory header for using write syscall
#include <stdio.h>  // mandatory for sprintf
#include <string.h> // mandatory for string operations

// defining macro for stdout file descriptor
#define STDOUT_FD   1 

// variadic function implementation (ellipsis must be at the end)
void myPrintf(const char *format, ...) 
{
    // defining a ptr to variable arg list
    va_list args;

    // initialize the start of the variable arg list with the mandatory argument "format"
    va_start(args, format);

    // defining buf and offset for the entire string
    char buf[1024];
    int offset = 0;

    // defining a char ptr to passed string to parse it character by character until null is reached
    const char *ptr;
    for (ptr = format; *ptr != '\0'; ptr++) 
    {
        // check if ptr points to a percentage sign to detect format specifiers
        if (*ptr == '%') 
        {
            // defining an empty temp buf
            char temp[128];
            // defining a variable to hold the length of the stored string
            int length = 0;

            // switch on the value of the format specifier
            switch (*(++ptr)) 
            {
                case 'd': // int
                    // access an argument from arg list (must specify the EXPECTED type as int)
                    int intval = va_arg(args, int); 
                    // convert intval into string and store it in temp && its length in length
                    length = sprintf(temp, "%d", intval); 
                break;
                
                case 'c': // char
                    // access an argument from arg list (must specify the EXPECTED type as char)
                    char charval = (char)va_arg(args, int); 
                    // can just store the char in temp and set length to 1 but used sprintf for consistency
                    length = sprintf(temp, "%c", charval); 
                break;
                
                case 's': // string
                    // access an argument from arg list (must specify the EXPECTED type as const char* aka string)
                    const char *strval = va_arg(args, const char *);
                    // copy strval in temp && its length in length
                    strcpy(temp, strval);
                    length = strlen(strval);
                break;
                
                case 'f': // float
                    // access an argument from arg list (must specify the EXPECTED type as float)
                    double floatval = va_arg(args, double); // got a warning and changed float into double
                    // convert floatval into string and store it in temp && its length in length
                    length = sprintf(temp, "%f", floatval);
                break;
                
                default: // just append printed string with % and what follows it
                    temp[0] = '%';
                    temp[1] = *ptr;
                    length = 2;
                break;
                
            }

            // if buf is not full => append it
            if (offset + length < sizeof(buf)) 
            {
                strcpy(&buf[offset], temp);
                offset += length;
            }
        } 
        else if (offset < sizeof(buf)) 
        {
            // just a normal char so append it into buf
            buf[offset++] = *ptr;
        }
    }

    // reset arg list ptr to null to prevent undefined behaviour
    va_end(args);

    // pass the buf to the syscall "write"
    write(STDOUT_FD, buf, offset); // stdout fd + str stored in buf + total length of str 
}

int main(void) 
{
    // defining a mix of variables to test myPrintf
    const char *str = "Hello From The Other Siiiide!";
    int x = 13;
    float y = 77.96;
    char z = 'Y';

    myPrintf("Tell me %c\n.", z);
    myPrintf("%d is an int && %f is a float\n.", x, y);
    myPrintf("%s I Must Have Called a Thousand Tiiiiiimes!\n.", str);
    
    return 1;
}
