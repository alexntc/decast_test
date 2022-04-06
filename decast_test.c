#include <stdio.h>
#include <string.h>

/* Target function here: new_token()
 * For test purposes I also made main() function so anyone can run a fast test of this program
 */

/* Errors */
#define STR_NO_ERR              0 // Input string correct
#define STR_LEN_ERROR           1 // Input string length error
#define STR_DATA_ERROR          2 // String contains unexpected symbols
#define STR_ERR_MAX_VAL         3 // The value of token string can't be increased (Z9-Z9-Z9-Z9-Z9-Z9-Z9-Z9-Z9-Z9)

/* @brief Checking if string contains unexpected symbols
 * @param *in_str: input symbol string
 * @param len: input string length
 * @retval STR_DATA_ERROR if unexpected symbols arrived, otherwise STR_NO_ERR
 */
int check_id_symbols_correct (char *in_str, int len)
{
    int ret = STR_NO_ERR;
    for (int i = 0; (i < len)&&(!ret); i++) {
        if (!(i%3)) {
            if ((in_str[i] > 'A') && (in_str[i] < '[')) {
                if ((in_str[i]=='E')||(in_str[i]=='I')||(in_str[i]=='O')||(in_str[i]=='U')) {
                    ret = STR_DATA_ERROR; // Unexpected symbol: vowel
                }
            } else {
                ret = STR_DATA_ERROR; // Unexpected symbol: must be letter here
            }
        } else if (i%3 == 1) {
            if ((in_str[i] < '0')||(in_str[i] > '9')) {
                ret = STR_DATA_ERROR; // Unexpected symbol: must be number here
            }
        } else if (i%3 == 2) {
            if (in_str[i] != '-') {
                ret = STR_DATA_ERROR; // Unexpected symbol: must be '-' here
            }
        }
    }
    return ret;
}

/* @brief Finding symbol to begin change with
 * @param *token_num: array contains info about numbers in tokens (if token is 1, number is 9)
 * @param *token_letter: array contains info about letters in tokens (if token_letter is 1, letter is Z)
 * @retval position of symbol in char array (string) to begin change with
 */
int find_change_begin_symbol (char *token_num, char *token_letter, int len)
{
    int chng_first_sym = 0;
    for (int i = (len + 1)/3; (i > 0) && (!chng_first_sym); i--) {
        if (token_num[i - 1]) {
            if (!token_letter[i - 1]) {
                chng_first_sym = i*3 - 2;
            }
        } else {
            chng_first_sym = i*3 - 1;
        }
    }
    return chng_first_sym;
}

/* @brief Forming next token string function
 * @param *in_str: input symbol string
 * @param *out_str: output symbol string
 * @param len: input string length
 * @retval error code if some error arrived, otherwise STR_NO_ERR
 */
int form_next_token (char *in_str, char *out_str, int len)
{
    int err_last_token = STR_NO_ERR;
    int tokens = (len+1)/3;
    char token_num_last[tokens], token_letter_last[tokens];
    /* Calculating info about potential changes into arrays */
    for (int i = 0; i < tokens; i++) {
        if (in_str[i*3] == 'Z'){
            token_letter_last[i] = 1;
        }
        if (in_str[i*3 + 1] == '9') {
            token_num_last[i] = 1;
        }
    }
    /* If only last symbol need to be changed, just copy all and increment last element */
    if (!token_num_last[tokens-1]) {
        memcpy(out_str, in_str, len);
        out_str[len-1]++;
    } else {
        /* If not only last symbol need to be changed, calculate position from which we need changes */
        int ch_beg_sym_idx = find_change_begin_symbol(token_num_last, token_letter_last, len);
        if (ch_beg_sym_idx != 0) {
            /* If not all string need to be changed: copy unchanged part, then change last part */
            memcpy(out_str, in_str, ch_beg_sym_idx - 1);
            int b1_start;
            if (ch_beg_sym_idx % 3 == 1) {
                out_str[ch_beg_sym_idx - 1] = (++in_str[ch_beg_sym_idx - 1]);
                if ((out_str[ch_beg_sym_idx - 1] == 'E')||(out_str[ch_beg_sym_idx - 1] == 'I')||(out_str[ch_beg_sym_idx - 1] == 'O')||(out_str[ch_beg_sym_idx - 1] == 'U')) {
                    out_str[ch_beg_sym_idx - 1]++;
                }
                out_str[ch_beg_sym_idx] = '1';
                b1_start = ch_beg_sym_idx + 1;
            } else {
                out_str[ch_beg_sym_idx - 1] = (++in_str[ch_beg_sym_idx - 1]);
                b1_start = ch_beg_sym_idx;
            }
            for (int i = b1_start; i < len; i+=3) {
                out_str[i] = '-';
                out_str[i+1] = 'B';
                out_str[i+2] = '1';
            }
        } else {
            /* If all string will be changed, all string will be made from B1 tokens */
            if (len < 29){
                for (int i = 0; i < (len + 3); i+=3) {
                    out_str[i] = 'B';
                    out_str[i + 1] = '1';
                    if (i != (len + 1)) {
                        out_str[i + 2] = '-';
                    }
                }
            } else {
                err_last_token = STR_ERR_MAX_VAL; // Error: we reached the maximum value, can't add anything
            }
        }
    }
    return err_last_token;
}

/* @brief Error checking and forming new token string
 * @param *str_input: input symbol string
 * @param *str_next: output symbol string
 * @retval error code if some error arrived, otherwise STR_NO_ERR
 */
int new_token (char *str_input, char *str_next)
{
    int err = STR_NO_ERR;
    int str_len = strlen(str_input);
    /* Length check */
    if ((str_len + 1)%3 || (str_len < 2) || (str_len > 29)) {
        err = STR_LEN_ERROR;
    }
    if (!err) {
        /* Check if symbols are in correct range */
        err = check_id_symbols_correct(str_input, str_len);
        if (!err) {
            /* Forming new token string */
            err = form_next_token(str_input, str_next, str_len);
        }
    }
    return err;
}

// Main
int main (int argc, char **argv)
{
    int err = 0;
    /* Checking arguments */
    if (argc != 2) {
        printf("This program should be launched as: program_exec INPUT_TOKEN_STRING (2 arguments)\n");
        return 1;
    }
    char str_output[30];
    memset (str_output, 0x0, 30);
    /* Trying to make new string */
    err = new_token(argv[1], str_output);
    /* Print error if occured */
    if (err) {
        printf("Error occured: ");
        switch (err){
        case STR_LEN_ERROR:
            printf ("Input string length error\n");
            break;
        case STR_DATA_ERROR:
            printf ("String contains unexpected symbols\n");
            break;
        case STR_ERR_MAX_VAL:
            printf ("The value of token string can't be increased\n");
            break;
        }
        return 1;
    }
    /* Print new string */
    printf("%s\n", str_output);
    return 0;
}