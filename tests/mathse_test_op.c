
#include <CUnit/Basic.h>
#include <CUnit/TestDB.h>

#include <stddef.h>

int
main (int argc, char **argv)
{
  if (argc == 1)
    {
      CU_basic_set_mode (CU_BRM_VERBOSE);
      CU_basic_run_tests ();
      CU_cleanup_registry ();
      return CU_get_error ();
    }
  else if (argc == 2)
    {
      char *sutie = argv[1];
      CU_pSuite pSuite = CU_get_suite (sutie);
      if (!pSuite)
        {
          return CU_get_error ();
        }
      else
        {
          CU_basic_set_mode (CU_BRM_VERBOSE);
          CU_basic_run_suite (pSuite);
          CU_cleanup_registry ();
          return CU_get_error ();
        }
    }
  else
    {
      return 0;
    }
}