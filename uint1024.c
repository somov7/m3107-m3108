#include <stdio.h>
#include <malloc.h>
#include <wchar.h> 
#include <string.h>

/**
 * @brief
 * the idea of this programm
 * is to convert given number to string
 * and keep it as a string till the end.
 * all numbers are represented in a string form.
 */
typedef struct
{
    char *string;
    short UndefinedBehavior;

} uint1024;

uint1024 from_uint(unsigned int x) // generation from unsigned int
{
    uint1024 generated;

    generated.string = (char *)(malloc(308 * sizeof(char))); // that's what i'll return after convertation

    char *str = (char *)(malloc(10 * sizeof(char))); // that's for unsigned int x

    for (int i = 9; i >= 0; i++)
    {
        str[i] = '0' + x % 10;
        x /= 10;
    }

    int detnull = 0;

    /**
     * i'm gonna find determinative null of number,
     *  so i'm gonna know string length
     */
    for (detnull; generated.string[detnull] != '\0'; detnull++)
        ;
    detnull--; // i'll take the index of last digit

    /**
     * i'll put given number representation in
     * the end of string, so it will be easier to use
     * math operations
     */
    for (int i = 0; detnull >= i; i++)
    {
        generated.string[307 - i] = generated.string[detnull - i];
    }

    /**
     * i'm gonna add insignificant nulls
     * in the begining of number representation
     * to prepare it to math operations
     */
    for (int i = 307 - detnull - 1; i >= 0; i--)
    {
        generated.string[i] = '0';
    }

    /**
     * i return number in uint1024
     */
    return generated;
}

/**
 * @brief
 * sometimes i dont really know
 * how big my number could,
 * especially after multiplication.
 * This function helps me to
 * find out what's the length of string,
 * what's the quantity of digits in your number
 * @param x
 * number in uint1024
 * @return string's length
 */
int length(uint1024 x)
{
    int counter = 0;

    for (int i = 0; x.string[i] >= '0' && x.string[i] <= '9'; i++)
        counter++;

    return counter;
}

/**
 * @brief
 * first of all i need to
 * find out what my number exactly is.
 * there 3 options: garbage like 123abc,
 * just a number like 999999, or just 0(not the same)
 * @param x
 * number in uint1024
 */
void printf_value(uint1024 x)
{
    int idx = 0; // to find out from when should i start printing digits

    /**
     * @brief
     * number could be zero, in this case i can return any digit,
     * 'cause all of them gon' be zeros(check out what generation from unsigned int do)
     */
    int IsZero = 1;

    int IsError = 1;

    /**
     * @brief
     * i'll check if i got error or complete zero
     */
    for (int i = 0; i < length(x); i++)
    {
        if (x.string[i] != '0' && IsZero == 1)
            IsZero = 0;

        if (x.string[i] != 'E' && IsError == 1)
            IsError = 0;
    }

    /**
     * @brief
     * i'll find first non-null digit
     */

    for (idx; x.string[idx] == '0' && idx < length(x); idx++)
        ;

    /**
     * @brief
     * if i have number 'zero' i'll print 0
     */
    if (IsZero && !IsError)
        printf("%c", x.string[0]);
    else
        for (int i = idx; i < length(x); i++) // otherwise i'll print number
            printf("%c", x.string[i]);
}

/**
 * @brief
 * this function checks if x is a number
 * @param x
 * x is a number
 * @return int
 * i'll return 1 (true) if it's nubmer, 0 (false) otherwise
 */
int CheckIfNumber(uint1024 *x)
{
    int flag = 1;

    for (int i = 0; i < 308; i++)
        if (!(x->string[i] >= '0' & x->string[i] <= '9'))
        {
            flag = 0;
            break;
        }

    if (flag)
        return 1;
    else
    {
        return 0;
    }
}

/**
 * @brief
 * scanf for uint1024
 * @param x
 */
void scanf_value(uint1024 *x)
{
    x->UndefinedBehavior = 0;

    /**
     * @brief
     * i'll allocate 309 bytes instead of 308
     * thus, if 308 byte is a digit between 0 or 9
     * then user entered too big number, that
     * datatype doesn't support
     */
    x->string = (char *)(malloc(309 * sizeof(char)));

    scanf("%s", x->string); // reading number

    int detnull = 0;

    for (detnull; x->string[detnull] != '\0'; detnull++) // i'll find where is the end of number
        ;
    detnull--;

    if (detnull != 307) // if number has less then 307 digits
    {
        for (int i = 0; detnull >= i; i++)
        {
            x->string[307 - i] = x->string[detnull - i]; // i'll represent number as 000000number
        }

        for (int i = 307 - detnull - 1; i >= 0; i--)
        {
            x->string[i] = '0';
        }

        x->string = (char *)(realloc(x->string, 308 * sizeof(char))); // so i can resize

        if (!CheckIfNumber(x))
        {
            x->UndefinedBehavior = 1;
            printf("\nIt's not a number\nUndefined behavior");
            return;
        }
    }

    else if (detnull == 308) // if number has 308 =>undef behavior
    {
        x->UndefinedBehavior = 1;
        printf("\nUndefined Behavior");
    }
    else
    {
        if (!CheckIfNumber(x))
        {
            x->UndefinedBehavior = 1;
            printf("\nIt's not a number\nUndefined behavior");
            return;
        }
        x->string = (char *)(realloc(x->string, 308 * sizeof(char))); // otherwise i can resize string
    }
}

/**
 * @brief
 * sum function for uint1024
 * @param num1
 * first number
 * @param num2
 * second number
 * @return uint1024
 * i'll return sum of first and second numbers
 */
uint1024 add_op(uint1024 *num1, uint1024 *num2)
{
    int DigitShift = 0;
    for (int i = 0; i < 308; i++)
    {
        int CurDigit = (num1->string[307 - i] - '0' + DigitShift + num2->string[307 - i] - '0'); // counting current digit
        if (i != 307)
        {
            DigitShift = CurDigit / 10; // it can happen to become more than 10

            num1->string[307 - i] = '0' + CurDigit % 10; // so i put digit%10 for now and remember shift for later
        }
        else if (CurDigit >= 10 && i == 307) // if it turned out that for first digit sum is equal or bigger than 10
        {
            printf("%s", "\nUndefined Behavior"); // then datatype overflows, 'cause now we're in need of one more digit, that we don't have
            num1->UndefinedBehavior = 1;
        }
        else
        {
            num1->string[307 - i] = '0' + CurDigit % 10; // otherwise i'll do the same as i did for other digits
        }
    }
    if (num1->UndefinedBehavior == 0) // if everything's okay, i'll print
        return *num1;
    else
    {
        memcpy(num1->string, "\nError\n", 307 * sizeof(char)); // otherwise i'll return error message
        return *num1;
    }
}

/**
 * @brief
 * subtraction function for uint1024
 * @param num1
 * first number
 * @param num2
 * second number
 * @return uint1024
 * i'll return difference between 2 numbers
 */
uint1024 subtr_op(uint1024 *num1, uint1024 *num2)
{
    for (int i = 0; i < 308; i++)
    {
        int CurDigit = (num1->string[307 - i] - num2->string[307 - i]); // calculating current digit
        if (i != 307)
        {
            if (CurDigit < 0) // if it turned out that digit is less than 0
            {
                num1->string[307 - i - 1] -= 1; // then i'll borrow 1 from neighboring digit

                num1->string[307 - i] = '0' + 10 + CurDigit; // then calculate current digit summing up 10 and current digit
            }
            else
            {
                num1->string[307 - i] = '0' + CurDigit; // otherwise i can evaluate result like 7-4=3
            }
        }
        else if (CurDigit < 0) // if current digit for first digit i less than 0
        {
            printf("%s", "\nUndefined Behavior"); // then difference is negative => undefined behavior
            num1->UndefinedBehavior = 1;
        }
    }
    if (num1->UndefinedBehavior == 0) // if everything's okay i'll return difference
    {
        return *num1;
    }
    else
    {
        memcpy(num1->string, "\nError\n", 307 * sizeof(char)); // if something went wrong i'll return error msg
        return *num1;
    }
}

/**
 * @brief
 * multiplication for uint1024
 * @param num1
 * first number
 * @param num2
 * second number
 * @return uint1024
 * i'll return product of first and second numbers
 */
uint1024 mult_op(uint1024 *num1, uint1024 *num2)
{
    int DigitShift = 0;

    int CurDigit = 0, SectionDigitShift = 0;

    char *result = (char *)(malloc(615 * sizeof(char))); // i'll allocate 615 bytes to accommodate result

    for (int i = 0; i < 615; i++) // initialize with '0'
        result[i] = '0';

    for (int i = 307; i >= 0; i--) // i'll run through numbers with nested loop
    {
        if (num1->UndefinedBehavior == 1)
            break;

        for (int j = 307; j >= 0; j--)
            if (i + j != 307)
            {
                CurDigit = (int)((num1->string[i] - '0') * (num2->string[j] - '0')); // evaluate current

                SectionDigitShift = (int)(result[i + j] - '0') + CurDigit % 10 + DigitShift; // current digit can overflow

                result[i + j] = '0' + (SectionDigitShift) % 10; // so i'll put sectiondigit%10

                DigitShift = CurDigit / 10 + SectionDigitShift / 10; // and will remember digit shift

                SectionDigitShift = 0;
            }
            else
            {
                CurDigit = (int)((num1->string[i] - '0') * (num2->string[j] - '0'));

                SectionDigitShift = (int)(result[i + j] - '0') + CurDigit % 10 + DigitShift;

                if (SectionDigitShift > 10) // if number overflows on 307 digit
                {
                    printf("\nUndefined Behavior"); // then error
                    num1->UndefinedBehavior = 1;
                    break;
                }
                else
                    result[i + j] = '0' + (SectionDigitShift) % 10; //else i'll evaluate current digit

                DigitShift = CurDigit / 10 + SectionDigitShift / 10; 

                SectionDigitShift = 0;
            }
    }

    if (num1->UndefinedBehavior == 0) //if everything's okay
    {
        for (int i = 0; i <= 307; i++) //i'll put 614 to 307, 613 to 306 etc
        {
            result[i] = result[i + 307];
            result[i + 307] = '0';
        }

        for (int i = 0; i < 308; i++) //rewrite it in 1 of given numbers 
        {
            num1->string[i] = result[i];
        }

        free(result);   //gonna delete product result

        result = NULL;  //i dont need pointer anymore

        return *num1;  //will return product
    }
    else    //if everything's bad
    {
        memcpy(num1->string, "\nError\n", 307 * sizeof(char)); //i'll return error msg
        return *num1;
    }
}

int main()
{
    uint1024 number1;
    uint1024 number2;
    char sign;

    scanf_value(&number1);
    scanf("%s", &sign);
    scanf_value(&number2);

    switch (sign)   //calculator for 2 nums
    {
    case '+':
        printf_value(add_op(&number1, &number2));
        break;

    case '-':
        printf_value(subtr_op(&number1, &number2));
        break;

    case '*':
        printf_value(mult_op(&number1, &number2));
        break;
    }

    free(number1.string);   //make sure that there's no memory leak
    number1.string = NULL;

    free(number2.string);
    number2.string = NULL;

    return 0;
}