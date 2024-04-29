
#ifndef CI_RESULT_H_
#define CI_RESULT_H_


typedef enum ci_result_t
{
  CI_SUCCESS = 0,  // No error, success

  CI_ERR_INVALID,  // Invalid operation
  CI_ERR_ALLOC,    // Memory allocation error
  CI_ERR_FILE,     // Error opening a file
  CI_ERR_OUT,      // Error writing to a file
  CI_ERR_IN,       // Error reading from a file
}
ci_result_t;


#endif // CI_RESULT_H_
