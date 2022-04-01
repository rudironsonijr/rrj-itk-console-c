#include <fclasses/tc_string.h>
#include <pom/enq/enq.h>
#include <schmgt/schmgt_itk.h>
#include <schmgt/schmgt_bridge_itk.h>
#include <tc/emh.h>
#include <tc/tc_startup.h>
#include <tc/tc_errors.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/grm.h>
#include <tccore/tctype.h>
#include <tcinit/tcinit.h>

int getSchedules(char *key_attribute, char *key_value, int *num_schedules, tag_t *schedules);

// int getScheduleTask(
// 	std::string object_name,
// 	tag_t* schedule_task_tag
// 	);

// int criarDependencia(
// 	tag_t sucessora,
// 	tag_t predecessora,
// 	int depType,
// 	long lagTime,
// 	tag_t *taskDep
// 	);

// int criarDependencia2(
// 	tag_t sucessora,
// 	tag_t predecessora,
// 	int depType,
// 	long lagTime,
// 	tag_t *taskDep
// 	);

// int criarDependencia3(
// 	// tag_t schedule_tag,
// 	tag_t sucessora_task_tag,
// 	tag_t predecessora_task_tag,
// 	int depType,
// 	long lagTime,
// 	tag_t *taskDep
// 	);