#ifndef _BUILTINTESTSDEF_H_
#define _BUILTINTESTSDEF_H_

#include <cstdint>

#define getBuiltInTestResult(regValue, idxResult) ((builtInTestResult_t)((regValue >> (2 * idxResult)) & 0x00000003))

#define BUILT_IN_TEST_PROC_REG_COUNT			4
#define BUILT_IN_TEST_OUTPUT_REG_COUNT			2
#define BUILT_IN_TEST_STORAGE_REG_COUNT			2
#define BUILT_IN_TEST_REG_COUNT					(BUILT_IN_TEST_PROC_REG_COUNT + BUILT_IN_TEST_OUTPUT_REG_COUNT + BUILT_IN_TEST_STORAGE_REG_COUNT)

#define BUILT_IN_TEST_PROC_GBL_RES_REG_IDX		0
#define BUILT_IN_TEST_OUTPUT_GBL_RES_REG_IDX	4
#define BUILT_IN_TEST_STORAGE_GBL_RES_REG_IDX	6

#define BUILT_IN_TEST_COUNT_IN_REG				16

typedef enum {
	BITR_Not_Applicable = 0,
	BITR_Pending,
	BITR_Passed,
	BITR_Failed
} builtInTestResult_t;

char *BuiltInTestResultToString(builtInTestResult_t builtInTestResult);
void DisplayBuiltInTestsResults(uint32_t builtInTestsResultsRegValue, uint32_t builtInTestsResultsRegIndex);

#endif // _BUILTINTESTSDEF_H_
