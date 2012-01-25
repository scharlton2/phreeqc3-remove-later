#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <iostream>
#include <fstream>
#include <sstream>

#include "Utils.h"
#include "Phreeqc.h"
#include "Phreeqc_class.h"
#include "Parser.h"
#include "Solution.h"
#include "Exchange.h"
#include "Surface.h"
#include "PPassemblage.h"
#include "cxxKinetics.h"
#include "SSassemblage.h"
#include "GasPhase.h"
#include "Reaction.h"
#include "cxxMix.h"
#include "Temperature.h"
#include "dumper.h"
#include "runner.h"
#include "cxxMix.h"
#include "phqalloc.h"



/* ---------------------------------------------------------------------- */
int Phreeqc::
read_solution_raw(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads SOLUTION_RAW data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;
	/*
	 *  Accumulate lines in std string
	 */
	std::string keywordLines("");

	keywordLines.append(line);
	keywordLines.append("\n");
/*
 *   Read additonal lines
 */
	int save_echo_input = pr.echo_input;
	pr.echo_input = FALSE;
	for (;;)
	{
		return_value = check_line("solution_raw", TRUE, TRUE, TRUE, FALSE);
		/* empty, eof, keyword, print */
		if (return_value == EOF || return_value == KEYWORD)
			break;
		keywordLines.append(line);
		keywordLines.append("\n");
	}
	pr.echo_input = save_echo_input;

	std::istringstream iss_in(keywordLines);
	CParser parser(iss_in, phrq_io);
	assert(!reading_database());
	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}
	//For testing, need to read line to get started
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);


	cxxSolution sol(phrq_io);
	sol.read_raw(parser);
	//struct solution *soln_ptr = sol.cxxSolution2solution(PHREEQC_THIS);
	struct solution *soln_ptr = cxxSolution2solution(&sol);
	int n;

	/*
	 *  This is not quite right, may not produce sort order, forced sort
	 */

	if (solution_bsearch(soln_ptr->n_user, &n, FALSE) != NULL)
	{
		solution_free(solution[n]);
		solution[n] = soln_ptr;
	}
	else
	{
		n = count_solution++;
		if (count_solution >= max_solution)
		{
			space((void **) ((void *) &(solution)), count_solution,
				  &max_solution, sizeof(struct solution *));
		}
		solution[n] = soln_ptr;
		solution_sort();
	}

	// Need to output the next keyword
	if (return_value == KEYWORD) echo_msg(sformatf( "\t%s\n", line));
	return (return_value);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
read_surface_raw(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads SURFACE_RAW data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;
	/*
	 *  Accumulate lines in std string
	 */
	std::string keywordLines("");

	keywordLines.append(line);
	keywordLines.append("\n");
/*
 *   Read additonal lines
 */
	int save_echo_input = pr.echo_input;
	pr.echo_input = FALSE;
	for (;;)
	{
		return_value = check_line("surface_raw", TRUE, TRUE, TRUE, FALSE);
		/* empty, eof, keyword, print */
		if (return_value == EOF || return_value == KEYWORD)
			break;
		keywordLines.append(line);
		keywordLines.append("\n");
	}
	pr.echo_input = save_echo_input;

	std::istringstream iss_in(keywordLines);
	CParser parser(iss_in, phrq_io);
	assert(!reading_database());
	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}
	//For testing, need to read line to get started
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	cxxSurface ex(phrq_io);
	ex.read_raw(parser);
	//struct surface *surface_ptr = ex.cxxSurface2surface(PHREEQC_THIS);
	struct surface *surface_ptr = cxxSurface2surface(&ex);
	int n;

	/*
	 *  This is not quite right, may not produce sort order, forced sort
	 */

	if (surface_bsearch(surface_ptr->n_user, &n) != NULL)
	{
		surface_free(&surface[n]);
		surface_copy(surface_ptr, &surface[n], surface_ptr->n_user);
	}
	else
	{
		n = count_surface++;
		if (count_surface >= max_surface)
		{
			space((void **) ((void *) &(surface)), count_surface,
				  &max_surface, sizeof(struct surface));
		}
		surface_copy(surface_ptr, &surface[n], surface_ptr->n_user);
		surface_sort();
	}
	surface_free(surface_ptr);
	free_check_null(surface_ptr);

	// Need to output the next keyword
	if (return_value == KEYWORD) echo_msg(sformatf( "\t%s\n", line));
	return (return_value);
}
#ifdef SKIP
/* ---------------------------------------------------------------------- */
int Phreeqc::
read_kinetics_raw(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads KINETICS_RAW data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;
	/*
	 *  Accumulate lines in std string
	 */
	std::string keywordLines("");

	keywordLines.append(line);
	keywordLines.append("\n");
/*
 *   Read additonal lines
 */
	int save_echo_input = pr.echo_input;
	pr.echo_input = FALSE;
	for (;;)
	{
		return_value = check_line("kinetics_raw", TRUE, TRUE, TRUE, FALSE);
		/* empty, eof, keyword, print */
		if (return_value == EOF || return_value == KEYWORD)
			break;
		keywordLines.append(line);
		keywordLines.append("\n");
	}
	pr.echo_input = save_echo_input;

	std::istringstream iss_in(keywordLines);
	CParser parser(iss_in, phrq_io);
	assert(!reading_database());
	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}
	//For testing, need to read line to get started
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	cxxKinetics ex(phrq_io);
	ex.read_raw(parser);
	//struct kinetics *kinetics_ptr = ex.cxxKinetics2kinetics(PHREEQC_THIS);
	struct kinetics *kinetics_ptr = cxxKinetics2kinetics(&ex);
	int n;

	/*
	 *  This is not quite right, may not produce sort order, forced sort
	 */

	if (kinetics_bsearch(kinetics_ptr->n_user, &n) != NULL)
	{
		kinetics_free(&kinetics[n]);
		kinetics_copy(kinetics_ptr, &kinetics[n], kinetics_ptr->n_user);
	}
	else
	{
		n = count_kinetics++;
		if (count_kinetics >= max_kinetics)
		{
			space((void **) ((void *) &(kinetics)), count_kinetics,
				  &max_kinetics, sizeof(struct kinetics));
		}
		kinetics_copy(kinetics_ptr, &kinetics[n], kinetics_ptr->n_user);
		kinetics_sort();
	}
	kinetics_free(kinetics_ptr);
	free_check_null(kinetics_ptr);

	// Need to output the next keyword
	if (return_value == KEYWORD) echo_msg(sformatf( "\t%s\n", line));
	return (return_value);
}
#endif
/* ---------------------------------------------------------------------- */
int Phreeqc::
read_dump(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads DUMP data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;
	/*
	 *  Make parser
	 */
	std::istringstream iss_in;
	return_value = streamify_to_next_keyword(iss_in);
	CParser parser(iss_in, phrq_io);
	assert(!reading_database());

	//For testing, need to read line to get started
	parser.set_echo_file(CParser::EO_NONE);
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}

	dump_info.Read(parser);

	// Need to output the next keyword
	if (return_value == OPTION_KEYWORD) echo_msg(sformatf( "\t%s\n", line));
	return (return_value);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
read_delete(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads DELETE data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;
	/*
	 *  Make parser
	 */
	std::istringstream iss_in;
	return_value = streamify_to_next_keyword(iss_in);
	CParser parser(iss_in, phrq_io);
	assert(!reading_database());

	//For testing, need to read line to get started
	parser.set_echo_file(CParser::EO_NONE);
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}

	StorageBinList deleter(parser, phrq_io);
	delete_info = deleter;


	// Need to output the next keyword
	if (return_value == OPTION_KEYWORD) echo_msg(sformatf( "\t%s\n", line));
	return (return_value);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
read_run_cells(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads DELETE data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;
	/*
	 *  Make parser
	 */
	std::istringstream iss_in;
	return_value = streamify_to_next_keyword(iss_in);
	CParser parser(iss_in, phrq_io);
	assert(!reading_database());

	//For testing, need to read line to get started
	parser.set_echo_file(CParser::EO_NONE);
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}

	runner r(parser, phrq_io);
	run_info = r;


	// Need to output the next keyword
	if (return_value == OPTION_KEYWORD) echo_msg(sformatf( "\t%s\n", line));
	return (return_value);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
read_solution_modify(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads solution_modify data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;

	// find solution number
	char token[MAX_LENGTH];
	char *next;
	int l, n_user, n;
	next = line;
	copy_token(token, &next, &l);
	if (copy_token(token, &next, &l) != DIGIT)
	{
		input_error++;
		error_string = sformatf( "Expected solution number following SOLUTION_MODIFY.\n%s\n", line_save);
		error_msg(error_string, CONTINUE);
		std::istringstream iss_in;
		return_value = streamify_to_next_keyword(iss_in);
		return (ERROR);
	} 
	else
	{
		sscanf(token,"%d", &n_user);
	}	
	/*
	 *  Make parser
	 */
	std::istringstream iss_in;
	return_value = streamify_to_next_keyword(iss_in);
	CParser parser(iss_in, phrq_io);
	assert(!reading_database());

	//For testing, need to read line to get started
	parser.set_echo_file(CParser::EO_NONE);
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}


	if (solution_bsearch(n_user, &n, FALSE) == NULL)
	{
		input_error++;
		error_string = sformatf( "Solution %d not found for SOLUTION_MODIFY.\n", n_user);
		error_msg(error_string, CONTINUE);
		return (ERROR);
	}

	cxxSolution sol(solution[n], phrq_io);

	// Clear activities so we can know what was read
	sol.clear_master_activity();

	sol.read_raw(parser, false);

	cxxSolution orig(solution[n], phrq_io );

	sol.modify_activities(orig);

	//struct solution *soln_ptr = sol.cxxSolution2solution(PHREEQC_THIS);
	struct solution *soln_ptr = cxxSolution2solution(&sol);

	/*
	 *  This is not quite right, may not produce sort order, forced sort
	 */

	solution_free(solution[n]);
	solution[n] = soln_ptr;

	// Need to output the next keyword
	if (return_value == OPTION_KEYWORD) echo_msg(sformatf( "\t%s\n", line));
	return (return_value);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
read_surface_modify(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads SURFACE_MODIFY data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;

	// find surface number
	char token[MAX_LENGTH];
	char *next;
	int l, n_user, n;
	next = line;
	copy_token(token, &next, &l);
	if (copy_token(token, &next, &l) != DIGIT)
	{
		input_error++;
		error_string = sformatf( "Expected surface number following SURFACE_MODIFY.\n%s\n", line_save);
		error_msg(error_string, CONTINUE);
		std::istringstream iss_in;
		return_value = streamify_to_next_keyword(iss_in);
		return (ERROR);
	} 
	else
	{
		sscanf(token,"%d", &n_user);
	}
	/*
	 *  Make parser
	 */
	std::istringstream iss_in;
	return_value = streamify_to_next_keyword(iss_in);
	CParser parser(iss_in, phrq_io);
	assert(!reading_database());

	//For testing, need to read line to get started
	parser.set_echo_file(CParser::EO_NONE);
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}

	if (surface_bsearch(n_user, &n) == NULL)
	{
		input_error++;
		error_string = sformatf( "Surface %d not found for SURFACE_MODIFY.\n", n_user);
		error_msg(error_string, CONTINUE);
		return (ERROR);
	}

	// read entity
	cxxSurface entity(&(surface[n]), phrq_io);
	entity.read_raw(parser, false);

	// save entity
	//struct surface *entity_ptr = entity.cxxSurface2surface(PHREEQC_THIS);
	struct surface *entity_ptr = cxxSurface2surface(&entity);
	surface_free(&(surface[n]));
	surface_copy(entity_ptr, &(surface[n]), entity_ptr->n_user);
	free_check_null(surface[n].description);
	surface[n].description = string_duplicate(entity_ptr->description);
	surface_free(entity_ptr);
	free_check_null(entity_ptr);

	// Need to output the next keyword
	if (return_value == OPTION_KEYWORD) echo_msg(sformatf( "\t%s\n", line));
	return (return_value);
}
#ifdef SKIP
/* ---------------------------------------------------------------------- */
int Phreeqc::
read_kinetics_modify(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads KINETICS_MODIFY data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;

	// find kinetics number
	char token[MAX_LENGTH];
	char *next;
	int l, n_user, n;
	next = line;
	copy_token(token, &next, &l);
	if (copy_token(token, &next, &l) != DIGIT)
	{
		input_error++;
		error_string = sformatf( "Expected kinetics number following KINETICS_MODIFY.\n%s\n", line_save);
		error_msg(error_string, CONTINUE);
		std::istringstream iss_in;
		return_value = streamify_to_next_keyword(iss_in);
		return (ERROR);
	} 
	else
	{
		sscanf(token,"%d", &n_user);
	}
	/*
	 *  Make parser
	 */
	std::istringstream iss_in;
	return_value = streamify_to_next_keyword(iss_in);
	CParser parser(iss_in, phrq_io);
	assert(!reading_database());

	//For testing, need to read line to get started
	parser.set_echo_file(CParser::EO_NONE);
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}

	if (kinetics_bsearch(n_user, &n) == NULL)
	{
		input_error++;
		error_string = sformatf( "Kinetics %d not found for KINETICS_MODIFY.\n", n_user);
		error_msg(error_string, CONTINUE);
		return (ERROR);
	}

	// read entity
	cxxKinetics entity(&(kinetics[n]), phrq_io);
	entity.read_raw(parser, false);

	// save entity
	//struct kinetics *entity_ptr = entity.cxxKinetics2kinetics(PHREEQC_THIS);
	struct kinetics *entity_ptr = cxxKinetics2kinetics(&entity);
	kinetics_free(&(kinetics[n]));
	kinetics_copy(entity_ptr, &(kinetics[n]), entity_ptr->n_user);
	free_check_null(kinetics[n].description);
	kinetics[n].description = string_duplicate(entity_ptr->description);
	kinetics_free(entity_ptr);
	free_check_null(entity_ptr);

	// Need to output the next keyword
	if (return_value == OPTION_KEYWORD) echo_msg(sformatf( "\t%s\n", line));
	return (return_value);
}
#endif
/* ---------------------------------------------------------------------- */
int Phreeqc::
streamify_to_next_keyword(std::istringstream & lines)
/* ---------------------------------------------------------------------- */
{
/*
 *   Reads to next keyword or eof
 *
 *   Returns:
 *       OPTION_KEYWORD
 *       OPTION_EOF
 *       
 */
	// Handle echo
	int save_echo_input = pr.echo_input;
	pr.echo_input = FALSE;

	std::string accumulate(line);
	accumulate.append("\n");
	int j;
	for (;;)
	{
		j = check_line("Streamify", FALSE, TRUE, TRUE, FALSE);
		if (j == EOF)
		{						/* end of file */
			break;
		}
		else if (j == KEYWORD)
		{						/* keyword */
			break;
		}
		else
		{
			accumulate.append(line);
			accumulate.append("\n");
		}
	}

	lines.str(accumulate);
	//std::cerr << accumulate;
	pr.echo_input = save_echo_input;
	if (j == EOF) return (OPTION_EOF);
	if (j == KEYWORD) return (OPTION_KEYWORD);


	return (OPTION_ERROR);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
dump_entities(void)
/* ---------------------------------------------------------------------- */
{
	if (!dump_info.Get_on())
	{
		return(OK);
	}
	dump_info.Set_on(false);
	if (!dump_info.Get_bool_any())
	{
		return(OK);
	}

	if (this->phrq_io)
	{
		std::ios_base::openmode mode = std::ios_base::out;
		if (dump_info.Get_append())
		{
			mode = std::ios_base::app;
		}
		if (this->phrq_io->dump_open(dump_info.Get_file_name().c_str(), mode))
		{
			dump_ostream(*this->phrq_io->Get_dump_ostream());
			this->phrq_io->dump_close();
		}
		else
		{
			error_string = sformatf( "Unable to open dump file \"%s\"", dump_info.Get_file_name().c_str());
			error_msg(error_string, STOP);
		}
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
delete_entities(void)
/* ---------------------------------------------------------------------- */
{
	int i, n, return_value;
	return_value = OK;
	if (!delete_info.Get_solution().Get_defined() &&
		!delete_info.Get_pp_assemblage().Get_defined() &&
		!delete_info.Get_exchange().Get_defined() &&
		!delete_info.Get_surface().Get_defined() &&
		!delete_info.Get_ss_assemblage().Get_defined() &&
		!delete_info.Get_gas_phase().Get_defined() &&
		!delete_info.Get_kinetics().Get_defined() &&
		!delete_info.Get_mix().Get_defined() &&
		!delete_info.Get_reaction().Get_defined() &&
		!delete_info.Get_temperature().Get_defined() &&
		!delete_info.Get_pressure().Get_defined())
	{
		return(OK);
	}

	// solutions
	if (delete_info.Get_solution().Get_defined())
	{
		if (delete_info.Get_solution().Get_numbers().size() == 0)
		{
			for (i = 0; i < count_solution; i++)
			{
				solution_delete_n(i);
			}
		}
		else
		{
			std::set < int >::iterator it;
			for (it = delete_info.Get_solution().Get_numbers().begin(); it != delete_info.Get_solution().Get_numbers().end(); it++)
			{
				if (solution_bsearch(*it, &n, FALSE) != NULL)
				{
					solution_delete_n(n);
				}
			}
		}
	}

	// pp_assemblages
	if (delete_info.Get_pp_assemblage().Get_defined())
	{
		if (delete_info.Get_pp_assemblage().Get_numbers().size() == 0)
		{
			Rxn_pp_assemblage_map.clear();
		}
		else
		{
			std::set < int >::iterator it;
			for (it = delete_info.Get_pp_assemblage().Get_numbers().begin(); it != delete_info.Get_pp_assemblage().Get_numbers().end(); it++)
			{
				Rxn_pp_assemblage_map.erase(*it);
			}
		}
	}
	// exchangers
	if (delete_info.Get_exchange().Get_defined())
	{
		if (delete_info.Get_exchange().Get_numbers().size() == 0)
		{
			Rxn_exchange_map.clear();
		}
		else
		{
			std::set < int >::iterator it;
			for (it = delete_info.Get_exchange().Get_numbers().begin(); it != delete_info.Get_exchange().Get_numbers().end(); it++)
			{
				Rxn_exchange_map.erase(*it);
			}
		}
	}
	// surfaces
	if (delete_info.Get_surface().Get_defined())
	{
		if (delete_info.Get_surface().Get_numbers().size() == 0)
		{
			for (i = 0; i < count_surface; i++)
			{
				surface_delete(surface[i].n_user);
			}
		}
		else
		{
			std::set < int >::iterator it;
			for (it = delete_info.Get_surface().Get_numbers().begin(); it != delete_info.Get_surface().Get_numbers().end(); it++)
			{
				if (surface_bsearch(*it, &n) != NULL)
				{
					surface_delete(*it);
				}
			}
		}
	}

	// ss_assemblages
	if (delete_info.Get_ss_assemblage().Get_defined())
	{
		if (delete_info.Get_ss_assemblage().Get_numbers().size() == 0)
		{
			Rxn_ss_assemblage_map.clear();
		}
		else
		{
			std::set < int >::iterator it;
			for (it = delete_info.Get_ss_assemblage().Get_numbers().begin(); it != delete_info.Get_ss_assemblage().Get_numbers().end(); it++)
			{
				Rxn_ss_assemblage_map.erase(*it);
			}
		}
	}
	// gas_phases
	if (delete_info.Get_gas_phase().Get_defined())
	{
		if (delete_info.Get_gas_phase().Get_numbers().size() == 0)
		{
			Rxn_gas_phase_map.clear();
		}
		else
		{
			std::set < int >::iterator it;
			for (it = delete_info.Get_gas_phase().Get_numbers().begin(); it != delete_info.Get_gas_phase().Get_numbers().end(); it++)
			{
				Rxn_gas_phase_map.erase(*it);
			}
		}
	}
	// kinetics
	if (delete_info.Get_kinetics().Get_defined())
	{
		if (delete_info.Get_kinetics().Get_numbers().size() == 0)
		{
			Rxn_kinetics_map.clear();
		}
		else
		{
			std::set < int >::iterator it;
			for (it = delete_info.Get_kinetics().Get_numbers().begin(); it != delete_info.Get_kinetics().Get_numbers().end(); it++)
			{
				Rxn_kinetics_map.erase(*it);
			}
		}
	}
#ifdef SKIP
	if (delete_info.Get_kinetics().Get_defined())
	{
		if (delete_info.Get_kinetics().Get_numbers().size() == 0)
		{
			for (i = 0; i < count_kinetics; i++)
			{
				kinetics_delete(kinetics[i].n_user);
			}
		}
		else
		{
			std::set < int >::iterator it;
			for (it = delete_info.Get_kinetics().Get_numbers().begin(); it != delete_info.Get_kinetics().Get_numbers().end(); it++)
			{
				if (kinetics_bsearch(*it, &n) != NULL)
				{
					kinetics_delete(*it);
				}
			}
		}
	}
#endif
	// mixes
	if (delete_info.Get_mix().Get_defined())
	{
		if (delete_info.Get_mix().Get_numbers().size() == 0)
		{
			Rxn_mix_map.clear();
		}
		else
		{
			std::set < int >::iterator it;
			for (it = delete_info.Get_mix().Get_numbers().begin(); it != delete_info.Get_mix().Get_numbers().end(); it++)
			{
				Rxn_mix_map.erase(*it);
			}
		}
	}
	// reactions
	if (delete_info.Get_reaction().Get_defined())
	{
		if (delete_info.Get_reaction().Get_numbers().size() == 0)
		{
			Rxn_reaction_map.clear();
		}
		else
		{
			std::set < int >::iterator it;
			for (it = delete_info.Get_reaction().Get_numbers().begin(); it != delete_info.Get_reaction().Get_numbers().end(); it++)
			{
				Rxn_reaction_map.erase(*it);
			}
		}
	}
	// temperatures
	if (delete_info.Get_temperature().Get_defined())
	{
		if (delete_info.Get_temperature().Get_numbers().size() == 0)
		{
			Rxn_temperature_map.clear();
		}
		else
		{
			std::set < int >::iterator it;
			for (it = delete_info.Get_temperature().Get_numbers().begin(); it != delete_info.Get_temperature().Get_numbers().end(); it++)
			{
				Rxn_temperature_map.erase(*it);
			}
		}
	}
	// pressures
	if (delete_info.Get_pressure().Get_defined())
	{
		if (delete_info.Get_pressure().Get_numbers().size() == 0)
		{
			Rxn_pressure_map.clear();
		}
		else
		{
			std::set < int >::iterator it;
			for (it = delete_info.Get_pressure().Get_numbers().begin(); it != delete_info.Get_pressure().Get_numbers().end(); it++)
			{
				Rxn_pressure_map.erase(*it);
			}
		}
	}
	// Turn off delete until next read
	delete_info.SetAll(false);
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
run_as_cells(void)
/* ---------------------------------------------------------------------- */
{
	struct save save_data;
	LDBLE kin_time;
	int count_steps, use_mix;
	char token[2 * MAX_LENGTH];
	//struct kinetics *kinetics_ptr;

	state = REACTION;
	if (run_info.Get_cells().Get_numbers().size() == 0 ||
		!(run_info.Get_cells().Get_defined())) return(OK);

	// running cells
	run_info.Set_run_cells(true);

	dup_print("Beginning of run as cells.", TRUE);
	LDBLE initial_total_time_save;
	if (run_info.Get_start_time() != NA)
	{
		initial_total_time_save = run_info.Get_start_time();
	}
	else
	{
		initial_total_time_save = initial_total_time;
	}

	std::set < int >::iterator it = run_info.Get_cells().Get_numbers().begin();

	for ( ; it != run_info.Get_cells().Get_numbers().end(); it++)
	{
		int i = *it;
		if (i < 0) continue;
		initial_total_time = initial_total_time_save;
		set_advection(i, TRUE, TRUE, i);
/*
 *   Run reaction step
 */
		/*
		*   Find maximum number of steps
		*/
		dup_print("Beginning of batch-reaction calculations.", TRUE);
		count_steps = 1;
		if (use.Get_reaction_in() == TRUE && use.Get_reaction_ptr() != NULL)
		{
			int count = ((cxxReaction *) use.Get_reaction_ptr())->Get_reaction_steps();
			if (count > count_steps)
				count_steps = count;
		}
		if (use.Get_kinetics_in() == TRUE && use.Get_kinetics_ptr() != NULL)
		{
			if (use.Get_kinetics_ptr()->Get_reaction_steps() > count_steps)
				count_steps = use.Get_kinetics_ptr()->Get_reaction_steps();
		}
		if (use.Get_temperature_in() == TRUE && use.Get_temperature_ptr() != NULL)
		{
			int count = ((cxxTemperature *) use.Get_temperature_ptr())->Get_countTemps();
			if (count > count_steps)
			{
				count_steps = count;
			}
		}
		if (use.Get_pressure_in() == TRUE && use.Get_pressure_ptr() != NULL)
		{
			int count = ((cxxPressure *) use.Get_pressure_ptr())->Get_count();
			if (count > count_steps)
			{
				count_steps = count;
			}
		}
		count_total_steps = count_steps;
		/*
		*  save data for saving solutions
		*/
		memcpy(&save_data, &save, sizeof(struct save));
		/* 
		*Copy everything to -2
		*/
		copy_use(-2);
		rate_sim_time_start = 0;
		rate_sim_time = 0;
		for (reaction_step = 1; reaction_step <= count_steps; reaction_step++)
		{
			sprintf(token, "Reaction step %d.", reaction_step);
			if (reaction_step > 1 && incremental_reactions == FALSE)
			{
				copy_use(-2);
			}
			set_initial_moles(-2);
			dup_print(token, FALSE);
			/*
			*  Determine time step for kinetics
			*/
			kin_time = 0.0;
			if (use.Get_kinetics_in() == TRUE)
			{
				// runner kin_time
				// equivalent to kin_time in count_steps
				if (run_info.Get_time_step() != NA)
				{
					if (incremental_reactions == FALSE)
					{
						/* not incremental reactions */
						kin_time = reaction_step * run_info.Get_time_step() / ((LDBLE) count_steps);
					}
					else
					{
						/* incremental reactions */
						kin_time = run_info.Get_time_step() / ((LDBLE) count_steps);
					}
				}
				// runner kin_time not defined
				else
				{
					cxxKinetics *kinetics_ptr = Utilities::Rxn_find(Rxn_kinetics_map, -2);
					kin_time = kinetics_ptr->Current_step((incremental_reactions==TRUE), reaction_step);

#ifdef SKIP
					kinetics_ptr = kinetics_bsearch(-2, &m);
					if (incremental_reactions == FALSE)
					{
						if (kinetics_ptr->count_steps > 0)
						{
							if (reaction_step > kinetics_ptr->count_steps)
							{
								kin_time = kinetics_ptr->steps[kinetics_ptr->count_steps - 1];
							}
							else
							{
								kin_time = kinetics_ptr->steps[reaction_step - 1];
							}
						}
						else if (kinetics_ptr->count_steps < 0)
						{
							if (reaction_step > -kinetics_ptr->count_steps)
							{
								kin_time = kinetics_ptr->steps[0];
							}
							else
							{
								kin_time = reaction_step * kinetics_ptr->steps[0] /	((LDBLE) (-kinetics_ptr->count_steps));
							}
						}
					}
					else
					{
						/* incremental reactions */
						if (kinetics_ptr->count_steps > 0)
						{
							if (reaction_step > kinetics_ptr->count_steps)
							{
								kin_time = kinetics_ptr->steps[kinetics_ptr->count_steps - 1];
							}
							else
							{
								kin_time = kinetics_ptr->steps[reaction_step - 1];
							}
						}
						else if (kinetics_ptr->count_steps < 0)
						{
							if (reaction_step > -kinetics_ptr->count_steps)
							{
								kin_time = 0;
							}
							else
							{
								kin_time =
									kinetics_ptr->steps[0] / ((LDBLE) (-kinetics_ptr->count_steps));
							}
						}
					}
#endif
				}
			}
			if (incremental_reactions == FALSE ||
				(incremental_reactions == TRUE && reaction_step == 1))
			{
				use_mix = TRUE;
			}
			else
			{
				use_mix = FALSE;
			}
			/*
			*   Run reaction step
			*/
			run_reactions(-2, kin_time, use_mix, 1.0);
			if (incremental_reactions == TRUE)
			{
				rate_sim_time_start += kin_time;
				rate_sim_time = rate_sim_time_start;
			}
			else
			{
				rate_sim_time = kin_time;
			}
			if (state != ADVECTION)
			{
				punch_all();
				print_all();
			}
			/* saves back into -2 */
			if (reaction_step < count_steps)
			{
				saver();
			}
		}
		/*
		*   save end of reaction
		*/
		memcpy(&save, &save_data, sizeof(struct save));
		if (use.Get_kinetics_in() == TRUE)
		{
			//kinetics_duplicate(-2, use.Get_n_kinetics_user());
			Utilities::Rxn_copy(Rxn_kinetics_map, -2, use.Get_n_kinetics_user());
		}
		saver();
	}
	initial_total_time += rate_sim_time;
	run_info.Get_cells().Set_defined(false);
	// not running cells
	run_info.Set_run_cells(false);
	return (OK);
}
/* ---------------------------------------------------------------------- */
void Phreeqc::
dump_ostream(std::ostream& os)
/* ---------------------------------------------------------------------- */
{
	int i, n;

	// solutions
	if (dump_info.Get_bool_solution())
	{
		if (dump_info.Get_solution().size() == 0)
		{
			for (i = 0; i < count_solution; i++)
			{
					cxxSolution cxxsoln(solution[i], phrq_io);
					cxxsoln.dump_raw(os,0);
			}
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.Get_solution().begin(); it != dump_info.Get_solution().end(); it++)
			{
				if (solution_bsearch(*it, &n, FALSE) != NULL)
				{
					cxxSolution cxxsoln(solution[n], phrq_io);
					cxxsoln.dump_raw(os,0);
				}
			}
		}
	}

	// pp_assemblages
	if (dump_info.Get_bool_pp_assemblage())
	{
		if (dump_info.Get_pp_assemblage().size() == 0)
		{
			Utilities::Rxn_dump_raw(Rxn_pp_assemblage_map, os, 0);
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.Get_pp_assemblage().begin(); it != dump_info.Get_pp_assemblage().end(); it++)
			{
				cxxPPassemblage *p = Utilities::Rxn_find(Rxn_pp_assemblage_map, *it);

				if (p != NULL)
				{
					p->dump_raw(os, 0);
				}
			}
		}
	}
	// exchanges
	if (dump_info.Get_bool_exchange())
	{
		if (dump_info.Get_exchange().size() == 0)
		{
			Utilities::Rxn_dump_raw(Rxn_exchange_map, os, 0);
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.Get_exchange().begin(); it != dump_info.Get_exchange().end(); it++)
			{
				cxxExchange *p = Utilities::Rxn_find(Rxn_exchange_map, *it);

				if (p != NULL)
				{
					p->dump_raw(os, 0);
				}
			}
		}
	}

	// surfaces
	if (dump_info.Get_bool_surface())
	{
		if (dump_info.Get_surface().size() == 0)
		{
			for (i = 0; i < count_surface; i++)
			{
					cxxSurface cxxentity(&surface[i], phrq_io);
					cxxentity.dump_raw(os,0);
			}
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.Get_surface().begin(); it != dump_info.Get_surface().end(); it++)
			{

				if (surface_bsearch(*it, &n) != NULL)
				{
					cxxSurface cxxentity(&surface[n], phrq_io);
					cxxentity.dump_raw(os,0);
				}
			}
		}
	}

	// ss_assemblages
	if (dump_info.Get_bool_ss_assemblage())
	{
		if (dump_info.Get_ss_assemblage().size() == 0)
		{
			Utilities::Rxn_dump_raw(Rxn_ss_assemblage_map, os, 0);
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.Get_ss_assemblage().begin(); it != dump_info.Get_ss_assemblage().end(); it++)
			{
				cxxSSassemblage *p = Utilities::Rxn_find(Rxn_ss_assemblage_map, *it);

				if (p != NULL)
				{
					p->dump_raw(os, 0);
				}
			}
		}
	}
	// gas_phases
	if (dump_info.Get_bool_gas_phase())
	{
		if (dump_info.Get_gas_phase().size() == 0)
		{
			Utilities::Rxn_dump_raw(Rxn_gas_phase_map, os, 0);
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.Get_gas_phase().begin(); it != dump_info.Get_gas_phase().end(); it++)
			{
				cxxGasPhase *p = Utilities::Rxn_find(Rxn_gas_phase_map, *it);

				if (p != NULL)
				{
					p->dump_raw(os, 0);
				}
			}
		}
	}

	// kinetics
	if (dump_info.Get_bool_kinetics())
	{
		if (dump_info.Get_kinetics().size() == 0)
		{
			Utilities::Rxn_dump_raw(Rxn_kinetics_map, os, 0);
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.Get_kinetics().begin(); it != dump_info.Get_kinetics().end(); it++)
			{
				cxxKinetics *p = Utilities::Rxn_find(Rxn_kinetics_map, *it);

				if (p != NULL)
				{
					p->dump_raw(os, 0);
				}
			}
		}
	}
#ifdef SKIP
	if (dump_info.Get_bool_kinetics())
	{
		if (dump_info.Get_kinetics().size() == 0)
		{
			for (i = 0; i < count_kinetics; i++)
			{
					cxxKinetics cxxentity(&kinetics[i], phrq_io);
					cxxentity.dump_raw(os,0);
			}
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.Get_kinetics().begin(); it != dump_info.Get_kinetics().end(); it++)
			{

				if (kinetics_bsearch(*it, &n) != NULL)
				{
					cxxKinetics cxxentity(&kinetics[n], phrq_io);
					cxxentity.dump_raw(os,0);
				}
			}
		}
	}
#endif
	// mix
	if (dump_info.Get_bool_mix())
	{
		if (dump_info.Get_mix().size() == 0)
		{
			Utilities::Rxn_dump_raw(Rxn_mix_map, os, 0);
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.Get_mix().begin(); it != dump_info.Get_mix().end(); it++)
			{
				cxxMix *p = Utilities::Rxn_find(Rxn_mix_map, *it);

				if (p != NULL)
				{
					p->dump_raw(os, 0);
				}
			}
		}
	}

	// reaction
	if (dump_info.Get_bool_reaction())
	{
		if (dump_info.Get_reaction().size() == 0)
		{
			Utilities::Rxn_dump_raw(Rxn_reaction_map, os, 0);
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.Get_reaction().begin(); it != dump_info.Get_reaction().end(); it++)
			{
				cxxReaction *p = Utilities::Rxn_find(Rxn_reaction_map, *it);

				if (p != NULL)
				{
					p->dump_raw(os, 0);
				}
			}
		}
	}

	// temperature
	if (dump_info.Get_bool_temperature())
	{
		if (dump_info.Get_temperature().size() == 0)
		{
			Utilities::Rxn_dump_raw(Rxn_temperature_map, os, 0);
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.Get_temperature().begin(); it != dump_info.Get_temperature().end(); it++)
			{
				cxxTemperature *p = Utilities::Rxn_find(Rxn_temperature_map, *it);

				if (p != NULL)
				{
					p->dump_raw(os, 0);
				}
			}
		}
	}
	// pressure
	if (dump_info.Get_bool_pressure())
	{
		if (dump_info.Get_pressure().size() == 0)
		{
			Utilities::Rxn_dump_raw(Rxn_pressure_map, os, 0);
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.Get_pressure().begin(); it != dump_info.Get_pressure().end(); it++)
			{
				cxxPressure *p = Utilities::Rxn_find(Rxn_pressure_map, *it);

				if (p != NULL)
				{
					p->dump_raw(os, 0);
				}
			}
		}
	}
	// Turn off any reaction calculation
	os << "USE mix none" << "\n";
	os << "USE reaction none" << "\n";
	os << "USE reaction_temperature none" << "\n";
	os << "USE reaction_pressure none" << "\n";

	// Turn off dump until next read
	dump_info.SetAll(false);
}
#if defined MULTICHART
/* ---------------------------------------------------------------------- */
int Phreeqc::
read_user_graph_handler(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads USER_GRAPH_DATA_BLOCK data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;

	/*
	 *  Make parser
	 */
	std::istringstream iss_in;
	return_value = streamify_to_next_keyword(iss_in);
	//std::ostringstream oss_out;
	//std::ostringstream oss_err;
	//CParser parser(iss_in, oss_out, oss_err, phrq_io);
	CParser parser(iss_in, phrq_io);

	//For testing, need to read line to get started
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	//parser.get_option(vopts, next_char);

	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}

	assert(!reading_database());

	bool success = chart_handler.Read(PHREEQC_THIS_COMMA parser);

	// Need to output the next keyword
	if (return_value == OPTION_KEYWORD) echo_msg(sformatf( "\t%s\n", line));
	return (return_value);
}
#endif
