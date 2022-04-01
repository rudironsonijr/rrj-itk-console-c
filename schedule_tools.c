#include <schedule_tools.h>
#include <utils.h>

int getSchedules(char *key_attribute, char *key_value, int *num_schedules, tag_t *schedules)
{
	int ifail = ITK_ok;

	const char
		*objectType = "Schedule",
		*value = NULL,
		*enquiry = "Provedor de Schedules",
		*selectedAttrs[] = { "puid" };

	logical queryExists = false;
	int
		n_rows = 0,
		n_cols = 0;

	tag_t tagObj;
	void*** reports;

	ifail = POM_enquiry_does_query_exists(enquiry, &queryExists);
	if (queryExists)
	{
		ifail = POM_enquiry_delete(enquiry);
	}

	// Criação da enquiry (inquisição)
	ifail = POM_enquiry_create(enquiry);
	// Resultados devem ser distintos
	ifail = POM_enquiry_set_distinct(enquiry, true);

	// Setar campos de retorno da enquiry (inquisição)
	ifail = POM_enquiry_add_select_attrs(enquiry, objectType, 1, selectedAttrs);

	// Expressão 01
	ifail = POM_enquiry_set_string_expr(enquiry, "expr1", objectType, key_attribute, POM_enquiry_like, key_value);
	ifail = POM_enquiry_set_expr_proprety(enquiry, "expr1", POM_case_insensitive);

	// Expressão 03, vinculando a expressão 01 e 02
	//ifail = POM_enquiry_set_expr(enquiry, "expr3", "expr1", POM_enquiry_and, "expr2");
	ifail = POM_enquiry_set_where_expr(enquiry, "expr1");

	// Execute the enquiry
	ifail = POM_enquiry_execute(enquiry, &n_rows, &n_cols, &reports);

	for (int i = 0; i < n_rows; i++)
	{
		tagObj = *(tag_t*)(reports[i][0]);
		schedules->push_back(tagObj);
	}

	// Delete the enquiry
	ifail = POM_enquiry_delete(enquiry);

	return ifail;
}


// int getScheduleTask(
// 	std::string object_name,
// 	tag_t* schedule_task_tag
// 	)
// {
// 	ResultStatus ifail = ITK_ok;
// 	int ifail = ITK_ok;

// 	const char* enquiry = "Provedor de Schedules";
// 	const char* selectedAttrs[] = { "puid" };
// 	logical queryExists = false;
// 	char* value = NULL;

// 	int n_rows = 0;
// 	int n_cols = 0;

// 	tag_t tagObj;
// 	Teamcenter::scoped_smptr<void**> reports;

// 	std::vector<tag_t> validWorkspaceObjectSearchResult;
// 	std::string objectType = "ScheduleTask";

// 	try
// 	{
// 		ifail = POM_enquiry_does_query_exists(enquiry, &queryExists);
// 		if (queryExists)
// 		{
// 			ifail = POM_enquiry_delete(enquiry);
// 		}

// 		// Criação da enquiry (inquisição)
// 		ifail = POM_enquiry_create(enquiry);
// 		// Resultados devem ser distintos
// 		ifail = POM_enquiry_set_distinct(enquiry, true);

// 		// Setar campos de retorno da enquiry (inquisição)
// 		ifail = POM_enquiry_add_select_attrs(enquiry, objectType.c_str(), 1, selectedAttrs);

// 		// Expressão 01
// 		ifail = POM_enquiry_set_string_expr(enquiry, "expr1", objectType.c_str(), "object_name", POM_enquiry_like, object_name.c_str());
// 		ifail = POM_enquiry_set_expr_proprety(enquiry, "expr1", POM_case_insensitive);

// 		// Expressão 03, vinculando a expressão 01 e 02
// 		//ifail = POM_enquiry_set_expr(enquiry, "expr3", "expr1", POM_enquiry_and, "expr2");
// 		ifail = POM_enquiry_set_where_expr(enquiry, "expr1");

// 		// Execute the enquiry
// 		ifail = POM_enquiry_execute(enquiry, &n_rows, &n_cols, &reports);

// 		for (int i = 0; i < n_rows; i++)
// 		{
// 			tagObj = *(tag_t*)(reports[i][0]);
// 			*schedule_task_tag = tagObj;
// 			// char * value;
// 			// ifail = AOM_ask_value_string(tagObj, "object_name", &value);
// 			// printf("valor: %s", value);
// 		}

// 		// Delete the enquiry
// 		ifail = POM_enquiry_delete(enquiry);
// 	}
// 		catch (const IFail & ex)
// 	{
// 		return ex.ifail();
// 	}

// 	return ifail;
// }

// int criarDependencia(
// 	tag_t sucessora_task_tag,
// 	tag_t predecessora_task_tag,
// 	int depType,
// 	long lagTime,
// 	tag_t *taskDep
// 	)
// {
// 	// sucessora_task_tag é o 'primary_object'
//     // predecessora_task_tag é o 'secondary_object'

// 	ResultStatus rstat;
//     *taskDep = NULLTAG;
// 	int ifail = ITK_ok;

// 	try
//     {
// 		tag_t depTypeTag = NULLTAG;
//         int ifail = GRM_find_relation_type(TaskDependencyRelType, &depTypeTag);
//         if (ITK_ok != ifail)
//         {
//            return ifail;
//         }



//         tag_t relation_tag = NULLTAG;
//         if ( sucessora_task_tag != NULLTAG && predecessora_task_tag != NULLTAG && depTypeTag != NULLTAG )
//         {
//             ifail = GRM_find_relation(sucessora_task_tag, predecessora_task_tag, depTypeTag, &relation_tag);
//             if (ifail != ITK_ok)
//             {
//             //    j.out("Unable to find Relation." , ifail);
//             //    j.returnValue(ifail);
//             //    ERROR_store_initial_error( ERROR_severity_error, ifail );
//                ifail = NOT_OK;
//             }

//             if ( relation_tag == NULLTAG )
//             {
//                 const char *TDtype = TaskDependencyRelType;
//                 Teamcenter::TaskDependency* pTaskDependency = 0;

//                 Teamcenter::CreateInput *pTaskDependencyCreInput = static_cast<Teamcenter::CreateInput* >(Teamcenter::BusinessObjectRegistry::instance().createInputObject(TDtype, OPERATIONINPUT_CREATE));
//                 Teamcenter::scoped_ptr< Teamcenter::CreateInput> scopedInput( pTaskDependencyCreInput );
//                 if( pTaskDependencyCreInput == NULL)
//                 {
// 					ifail = 1;
//                     return ifail;
//                 }

//                 rstat = pTaskDependencyCreInput->setTag("primary_object",sucessora_task_tag,false);
//                 rstat = pTaskDependencyCreInput->setTag("secondary_object",predecessora_task_tag,false);
//                 rstat = pTaskDependencyCreInput->setInt("dependency_type",depType,false);
//                 rstat = pTaskDependencyCreInput->setInt("lag_time",lagTime,false);
//                 rstat = pTaskDependencyCreInput->setTag("relation_type",depTypeTag,false);
//                 //PR#6040965 - Set user_data
//                 rstat = pTaskDependencyCreInput->setTag("user_data",NULLTAG,false);


//                 pTaskDependency = static_cast<Teamcenter::TaskDependency*>(Teamcenter::BusinessObjectRegistry::instance().createBusinessObject(pTaskDependencyCreInput));
//                 if(pTaskDependency != NULL)
//                 {
//                     *taskDep = pTaskDependency->getTag();

//                     // if( WhatIfSession::isOn( pTaskDependency->getTag() ) )
//                     // {
//                     //     pTaskDependency->setFnd0WhatIfMode( 1 );
//                     // }
//                 }
//                 else
//                 {
// 					rstat = Teamcenter::ErrorStoreBase::getInstance().ask_last_ifail();
//                 }

//                 ifail = ITK_ok;
//                 // Save the relation tag
//                 ifail = GRM_save_relation(*taskDep);
//                 if ( ifail != ITK_ok )
//                 {
//                     // ERROR_store_initial_error( ERROR_severity_error, ifail );
//                     AOM_unlock(*taskDep);
//                     return NOT_OK;
//                 }
//                 rstat = AOM_unlock(*taskDep);
//             }

//         }


// 	}
// 	catch(IFail &ex)
//     {
//         //Teamcenter::ScheduleManager::schmgt::logger()->error( ERROR_line, ex.ifail(), "Exception in TaskDependencyImpl::createDependency" );
//         return NOT_OK;
//     }


// 	return ifail;
// }

// int criarDependencia2(
// 	// tag_t schedule_tag,
// 	tag_t sucessora_task_tag,
// 	tag_t predecessora_task_tag,
// 	int depType,
// 	long lagTime,
// 	tag_t *taskDep
// 	)
// {
// 	// sucessora_task_tag é o 'primary_object'
//     // predecessora_task_tag é o 'secondary_object'
// 	int ifail = ITK_ok;

// 	try
//     {
// 		int app_id;
// 		int app_code;

// 		ERROR_CHECK(POM_register_application("SCHMGTV100", "SCHMGTV100", &app_id, &app_code));
// 		ERROR_CHECK(POM_identify_application(app_id, app_code, TRUE));

// 		tag_t dependency_relation_type = NULLTAG;
//         ifail = GRM_find_relation_type(TaskDependencyRelType, &dependency_relation_type);
//         if (ITK_ok != ifail)
//         {
//            return ifail;
//         }

// 		tag_t grm_create_input = NULLTAG;
// 		ERROR_CHECK(TCTYPE_construct_create_input(dependency_relation_type, &grm_create_input));

// 		ERROR_CHECK(AOM_set_value_tag(grm_create_input, "primary_object", sucessora_task_tag));
// 		ERROR_CHECK(AOM_set_value_tag(grm_create_input, "secondary_object", predecessora_task_tag));
// 		ERROR_CHECK(AOM_set_value_tag(grm_create_input, "relation_type", dependency_relation_type));
// 		ERROR_CHECK(AOM_set_value_int(grm_create_input, "lag_time", lagTime));
// 		ERROR_CHECK(AOM_set_value_int(grm_create_input, "dependency_type", depType));
// 		ERROR_CHECK(AOM_set_value_tag(grm_create_input, "user_data", NULLTAG));

// 		tag_t relation = NULLTAG;
// 		ERROR_CHECK(TCTYPE_create_object(grm_create_input, &relation));
// 		ERROR_CHECK(AOM_save(relation));

// 	}
// 	catch(IFail &ex)
//     {
//         //Teamcenter::ScheduleManager::schmgt::logger()->error( ERROR_line, ex.ifail(), "Exception in TaskDependencyImpl::createDependency" );
//         return NOT_OK;
//     }


// 	return ifail;
// }

// int criarDependencia3(
// 	// tag_t schedule_tag,
// 	tag_t tPrimaryObj,
// 	tag_t tSecondaryObj,
// 	int depType,
// 	long lagTime,
// 	tag_t *taskDep
// 	)
// {
// 	// sucessora_task_tag é o 'primary_object'
//     // predecessora_task_tag é o 'secondary_object'
// 	int ifail = ITK_ok;
// 	std::string sClassName = "TaskDependency";
// 	std::string sRelName = "TaskDependency";

// 	try
//     {
// 		int app_id;
// 		int app_code;

// 		ERROR_CHECK(POM_register_application("SCHMGTV100", "SCHMGTV100", &app_id, &app_code));
// 		ERROR_CHECK(POM_identify_application(app_id, app_code, TRUE));

// 		// tag_t relation_type=NULL_TAG;
// 		// tag_t tClassID=NULLTAG, tNewRelation=NULLTAG;
// 		// tag_t tPrimAttrTag=NULLTAG, tSecAttrTag=NULLTAG, treltype=NULLTAG, tUsrData=NULLTAG;


// 		tag_t relation_type=NULLTAG;
// 		tag_t tClassID=NULLTAG;
// 		tag_t tNewRelation=NULLTAG;
// 		ifail=GRM_find_relation_type(sRelName.c_str(),&relation_type);
// 		ifail=POM_class_id_of_class(sRelName.c_str(), &tClassID);
// 		if(tClassID!=NULL)
// 		{
// 			ERROR_CHECK(POM_create_instance(tClassID,&tNewRelation));
// 		}

// 		tag_t tPrimAttrTag=NULLTAG;
// 		ERROR_CHECK(POM_attr_id_of_attr("primary_object", sClassName.c_str(), &tPrimAttrTag));
// 		if (tPrimAttrTag!=NULLTAG)
// 		{
// 			ERROR_CHECK(POM_set_attr_tag(1, &tNewRelation, tPrimAttrTag, tPrimaryObj));
// 		}

// 		tag_t tSecAttrTag=NULLTAG;
// 		ERROR_CHECK( POM_attr_id_of_attr("secondary_object", sClassName.c_str(), &tSecAttrTag));
// 		if(tSecAttrTag!=NULLTAG)
// 		{
// 			ERROR_CHECK(POM_set_attr_tag(1, &tNewRelation, tSecAttrTag, tSecondaryObj));
// 		}

// 		tag_t treltype=NULLTAG;
// 		ERROR_CHECK(POM_attr_id_of_attr("relation_type", sClassName.c_str(), &treltype));
// 		if(treltype!=NULLTAG)
// 		{
// 			ERROR_CHECK(POM_set_attr_tag(1, &tNewRelation, treltype, relation_type));
// 		}

// 		// dependency_type
// 		tag_t tdependency_type=NULLTAG;
// 		ERROR_CHECK(POM_attr_id_of_attr("dependency_type", sClassName.c_str(), &tdependency_type));
// 		if(tdependency_type!=NULLTAG)
// 		{
// 			ERROR_CHECK(POM_set_attr_int(1, &tNewRelation, tdependency_type, depType));
// 		}

// 		// lag_time
// 		tag_t tlag_time=NULLTAG;
// 		ERROR_CHECK(POM_attr_id_of_attr("lag_time", sClassName.c_str(), &tlag_time));
// 		if(tlag_time!=NULLTAG)
// 		{
// 			ERROR_CHECK(POM_set_attr_int(1, &tNewRelation, tlag_time, lagTime));
// 		}
// 		tag_t tUsrData=NULLTAG;
// 		ERROR_CHECK(POM_attr_id_of_attr("user_data", sClassName.c_str(), &tUsrData));
// 		if(tUsrData!=NULLTAG)
// 		{
// 			ERROR_CHECK(POM_set_attr_tag(1, &tNewRelation, tUsrData, NULLTAG));
// 		}

// 		tag_t tRequiresDigitalSign=NULLTAG;
// 		ERROR_CHECK(POM_attr_id_of_attr("fnd0RequiresDigitalSign", sClassName.c_str(), &tRequiresDigitalSign));
// 		if (tRequiresDigitalSign!=NULLTAG)
// 		{
// 			ERROR_CHECK(POM_set_attr_logical(1, &tNewRelation, tRequiresDigitalSign, false));
// 		}

// 		tag_t t_fnd0CopyStableID=NULLTAG;
// 		char* fnd0CopyStableID;
// 		ERROR_CHECK(POM_tag_to_uid(tPrimaryObj, &fnd0CopyStableID));
// 		ERROR_CHECK(POM_attr_id_of_attr("fnd0CopyStableID", sClassName.c_str(), &t_fnd0CopyStableID));
// 		if (t_fnd0CopyStableID!=NULLTAG)
// 		{
// 			ERROR_CHECK(POM_set_attr_string(1, &tNewRelation, t_fnd0CopyStableID, fnd0CopyStableID));
// 		}

// 		tag_t t_last_mod_date = NULLTAG;
// 		ERROR_CHECK(POM_attr_id_of_attr("last_mod_date", "WorkspaceObject", &t_last_mod_date));
// 		date_t last_mod_date_value;
// 		logical is_it_null;
// 		logical is_it_empty;
// 		ERROR_CHECK(POM_ask_attr_date(tSecondaryObj, t_last_mod_date, &last_mod_date_value, &is_it_null, &is_it_empty));
// 		tag_t fnd0CopyStableDate=NULLTAG;
// 		ERROR_CHECK(POM_attr_id_of_attr("fnd0CopyStableDate", sClassName.c_str(), &fnd0CopyStableDate));
// 		if (fnd0CopyStableDate!=NULLTAG)
// 		{
// 			ERROR_CHECK(POM_set_attr_date(1, &tNewRelation, fnd0CopyStableDate, last_mod_date_value));
// 		}


// 		ERROR_CHECK(POM_save_instances ( 1, &tNewRelation, false ));
// 		*taskDep = tNewRelation;
// 	}
// 	catch(IFail &ex)
//     {

//         return NOT_OK;
//     }


// 	return ifail;
// }


// int getSchedulesUsingDataProvider() {

// 	// std::string providerName ("P7mSchPropsProvider");
// 	// int startIndex = 0;
// 	// int maxToReturn = 100000;
// 	// int maxToLoad = 100000;
// 	// ResultStatus rStat;
// 	// // Criação do SearchCriteria
// 	// Teamcenter::OperationInput* searchCriteriaInput = Teamcenter::BusinessObjectRegistry::instance().createInputObject( "SearchCriteria", "OperationInput" );
// 	// if( searchCriteriaInput == NULL )
// 	// {
// 	// 	throw IFail();
// 	// }
// 	// // Parâmetro do Critério
// 	// std::vector< tag_t > searchCriteriaInputs;
// 	// std::vector< int > isNullVector;
// 	// rStat = searchCriteriaInput->setString( "name", "p7mSCHcodigo", false );
// 	// rStat = searchCriteriaInput->setString( "value", "*", false );
// 	// searchCriteriaInputs.push_back( searchCriteriaInput->getTag() );
// 	// isNullVector.push_back( 0 );
// 	// // Instância do searchOperationInput
// 	// Teamcenter::OperationInput* searchOperationInput = Teamcenter::BusinessObjectRegistry::instance().createInputObject( providerName.c_str(), "OperationInput" );
// 	// if( searchOperationInput == NULL )
// 	// {
// 	// 	throw IFail();
// 	// }
// 	// rStat = searchOperationInput->setString( "providerName", providerName.c_str(), false );
// 	// rStat = searchOperationInput->setTagArray( "searchCriteria", searchCriteriaInputs, &isNullVector );
// 	// rStat = searchOperationInput->setInt( "startIndex", startIndex, false );
// 	// rStat = searchOperationInput->setInt( "maxToReturn", maxToReturn, false );
// 	// rStat = searchOperationInput->setInt( "maxToLoad", maxToLoad, false );

// 	// // Instância do searchOperationOutput
// 	// Teamcenter::OperationInput* searchOperationOutput = Teamcenter::BusinessObjectRegistry::instance().createInputObject( providerName.c_str(), "OperationInput" );
// 	// if( searchOperationOutput == NULL )
// 	// {
// 	// throw IFail();
// 	// }

// 	// //////////////////////
// 	// //   PERFORM SEARCH //
// 	// //////////////////////
// 	// // Instância do Provider
// 	// Fnd0BaseProvider* provider = 0;
// 	// provider = dynamic_cast< Fnd0BaseProvider *> ( Teamcenter::BusinessObjectRegistry::instance().createBusinessObject( providerName ) ) ;
// 	// if( provider == 0 )
// 	// {
// 	// throw IFail();
// 	// }
// 	// rStat = provider->fnd0performSearch( searchOperationInput, searchOperationOutput );
// 	// std::vector<tag_t> searchResults;
// 	// std::vector<int> isNullVals;
// 	// rStat = searchOperationOutput->getTagArray( "searchResults", searchResults, isNullVals);
// 	// bool isNull = false;
// 	// rStat = searchOperationOutput->getInt( "totalFound", searchResponse6.totalFound, isNull);
// 	// rStat = searchOperationOutput->getInt( "totalLoaded", searchResponse6.totalLoaded, isNull );
// 	// rStat = searchOperationOutput->getInt( "currentFTSIndexRow", searchResponse6.cursor.endIndex, isNull );
// 	// rStat = searchOperationOutput->getInt( "currentFTSIndexRow", searchResponse6.endIndex, isNull );

// 	return ITK_ok;
// }