#include "FileManager.h"
#include "help.h"
using namespace std;




bool FileManager::isNumeric(const string & in)
{
	return all_of(in.begin(), in.end(), ::isdigit);
}

FileManager::FileManager()
{
	currentSection = 0;
	currentLocationCounter = 0;
	symTable.push_back(new SymTableEntry("UND", 0, 0, true));
	
}

void FileManager::openFile(char * fName)
{
	ifstream file(fName);
	if (file.is_open())
	{
		Line* line = new Line();
		bool end = false;
		int lineCounter = 0;
		while (!end)
		{
			line = new Line();
			file >> *line;
			lineCounter++;
			//gledamo prvo labelu, dal postoji
			if (line->existLabel())
			{
				SymTableEntry* tmp = findSymTable(line->getLabel());
				if (tmp != nullptr)
				{
					if(tmp->isDefined())
						throw Exception("##GRESKA##     ##DVOSTRUKA DEFINICIJA SIMBOLA#", lineCounter, line);  //greska, visestruka definicija simbola
					else
					{
						tmp->setValue(currentLocationCounter);
						tmp->setSectionNum(currentSection);
						tmp->setDefined(true);
					}
				}
				else
					symTable.push_back(new SymTableEntry(line->getLabel(), currentSection, currentLocationCounter, true));
			}


			//mnemonik, tu spadaju i direktive, i instrukcije, i sekcije
			if (line->existMnemonic())
			{
				
				if (line->isDirective())
				{
					MAP::iterator it = directivesMap.find(line->getMnemonic());
					
					vector<string> operandsVector = line->getOperands();
					if (it != directivesMap.end())
					{
						string mnemonic = it->first;
						switch (it->second)
						{
							
							//.char
						case 0:
							if (curSection->name.find(".bss") != string::npos)
								throw Exception("##GRESKA##    ##BSS SEKCIJA NE SME SADRZATI INICIJALIZOVANE PODATKE###", lineCounter, line);
							for (vector<string>::iterator itOperands = operandsVector.begin(); itOperands != operandsVector.end(); ++itOperands)
							{
								if (isNumeric(*itOperands))
								{
									curSection->insertValue(currentLocationCounter, stoi(*itOperands), 1);
									currentLocationCounter++;
								}
								else
								{
									curSection->insertValue(currentLocationCounter, (*itOperands)[0], 1);
									currentLocationCounter++;
								}
							}
							break;
							//.wordh
						case 1:
							if (curSection->name.find(".bss") != string::npos)
								throw Exception("##GRESKA##    ##BSS SEKCIJA NE SME SADRZATI INICIJALIZOVANE PODATKE###", lineCounter, line);
							for (vector<string>::iterator itOperands = operandsVector.begin(); itOperands != operandsVector.end(); ++itOperands)
							{
								if (isNumeric(*itOperands))
								{
									int tmp = stoi(*itOperands);
									tmp >>= 16;
									curSection->insertValue(currentLocationCounter, tmp, 2);
								}
								else
								{
									//u pitanju je neka labela
									SymTableEntry* tmp = findSymTable(*itOperands);
									if (tmp != nullptr)
									{
										//ako je def onda uzimamo vrednost, ako nije, dodajemo u forward
										if (tmp->isDefined())
										{
											if (tmp->isGlobal())
											{
												curSection->insertValue(currentLocationCounter, 0, 2);
												curSection->retTable->insertValue(currentLocationCounter, 3, tmp->getCount());
											}
											else
											{
												int val = tmp->getValue();
												val >>= 16;
												curSection->insertValue(currentLocationCounter, val, 2);
												curSection->retTable->insertValue(currentLocationCounter, 3, tmp->getSectionNum());
											}
										}
										else
										{
											tmp->addFLink(currentLocationCounter, currentSection, 3);
											curSection->insertValue(currentLocationCounter, 0, 2);
										}

									}
									else
									{
										SymTableEntry* n = new SymTableEntry(*itOperands, 0, 0);
										n->addFLink(currentLocationCounter, currentSection, 3);
										symTable.push_back(n);
										curSection->insertValue(currentLocationCounter, 0, 2);
										//ubacujemo nov element u listu, nedefinisan
									}
								}
								currentLocationCounter += 2;
							}
							break;
							//wordl
						case 11:
							if (curSection->name.find(".bss") != string::npos)
								throw Exception("##GRESKA##    ##BSS SEKCIJA NE SME SADRZATI INICIJALIZOVANE PODATKE###", lineCounter, line);
							for (vector<string>::iterator itOperands = operandsVector.begin(); itOperands != operandsVector.end(); ++itOperands)
							{
								if (isNumeric(*itOperands))
								{
									int tmp = stoi(*itOperands);
									curSection->insertValue(currentLocationCounter, tmp, 2);
								}
								else
								{
									//u pitanju je neka labela
									SymTableEntry* tmp = findSymTable(*itOperands);
									if (tmp != nullptr)
									{
										//ako je def onda uzimamo vrednost, ako nije, dodajemo u forward
										if (tmp->isDefined())
										{
											if (tmp->isGlobal())
											{
												curSection->insertValue(currentLocationCounter, 0, 2);
												curSection->retTable->insertValue(currentLocationCounter, 4, tmp->getCount());
											}
											else
											{
												int val = tmp->getValue();
											    curSection->insertValue(currentLocationCounter, val, 2);
												curSection->retTable->insertValue(currentLocationCounter, 4, tmp->getSectionNum());
											}
										}
										else
										{
											tmp->addFLink(currentLocationCounter, currentSection, 4);
											curSection->insertValue(currentLocationCounter, 0, 2);
										}
									}
									else
									{
										SymTableEntry* n = new SymTableEntry(*itOperands, 0, 0);
										n->addFLink(currentLocationCounter, currentSection, 4);
										symTable.push_back(n);
										curSection->insertValue(currentLocationCounter, 0, 2);
										//ubacujemo nov element u listu, nedefinisan
									}
								}
								currentLocationCounter += 2;
							}
							break;
							//.long
						case 2:
							if (curSection->name.find(".bss") != string::npos)
								throw Exception("##GRESKA##    ##BSS SEKCIJA NE SME SADRZATI INICIJALIZOVANE PODATKE###", lineCounter, line);
							for (vector<string>::iterator itOperands = operandsVector.begin(); itOperands != operandsVector.end(); ++itOperands)
							{
								if (isNumeric(*itOperands))
								{
									curSection->insertValue(currentLocationCounter, stoi(*itOperands), 4);
								}
								else
								{
									//u pitanju je neka labela
									SymTableEntry* tmp = findSymTable(*itOperands);
									if (tmp != nullptr)
									{
										//ako je def onda uzimamo vrednost, ako nije, dodajemo u forward
										if (tmp->isDefined())
										{
											if (tmp->isGlobal())
											{
												curSection->insertValue(currentLocationCounter, 0);
												curSection->retTable->insertValue(currentLocationCounter, 1, tmp->getCount());
											}
											else
											{
												curSection->insertValue(currentLocationCounter, tmp->getValue());
												curSection->retTable->insertValue(currentLocationCounter, 1, tmp->getSectionNum());
											}
										}
										else
										{
											tmp->addFLink(currentLocationCounter, currentSection, 1);
											curSection->insertValue(currentLocationCounter, 0);
										}
									}
									else
									{
										SymTableEntry* n = new SymTableEntry(*itOperands, 0, 0);
										n->addFLink(currentLocationCounter, currentSection, 1);
										symTable.push_back(n);
										curSection->insertValue(currentLocationCounter, 0, 4);
										//ubacujemo nov element u listu, nedefinisan
									}
								}
								currentLocationCounter += 4;
							}
							break;
							//.align
						case 3:
							for (vector<string>::iterator itOperands = operandsVector.begin(); itOperands != operandsVector.end(); ++itOperands)
							{
								if (isNumeric(*itOperands))
								{
									//alignment treba da bude 2, 4, 8, stepen dvojke, da bi imalo smisla
									int alignment = stoi(*itOperands);
									int space = alignment- (currentLocationCounter % alignment);
									curSection->insertValue(currentLocationCounter, 0, space);
									currentLocationCounter += space;
								}
							}
							break;
							//.skip
						case 4:
							for (vector<string>::iterator itOperands = operandsVector.begin(); itOperands != operandsVector.end(); ++itOperands)
							{
								if (isNumeric(*itOperands))
								{
									int numOfBytesToSkip = stoi(*itOperands);
									curSection->insertValue(currentLocationCounter, 0, numOfBytesToSkip);
									currentLocationCounter += numOfBytesToSkip;
								}
							}
							break;
							//.global
						case 5:
							for (vector<string>::iterator itOperands = operandsVector.begin(); itOperands != operandsVector.end(); ++itOperands)
							{
								SymTableEntry* tmp = findSymTable(*itOperands);
								if (tmp != nullptr)
								{
									tmp->setVisibility('g');
								}
								else
								{
									SymTableEntry* newEntry = new SymTableEntry(*itOperands, currentSection, 0, false, 0, 'g');
									symTable.push_back(newEntry);
								}
							}
							break;
							//.extern
						case 6:
							for (vector<string>::iterator itOperands = operandsVector.begin(); itOperands != operandsVector.end(); ++itOperands)
							{
									//moze da se stavi za sekciju da je -1, to znaci da je extern i da ne sme da bude definisan negde taj simbol,
									//u suprotnom je greska
									SymTableEntry* newEntry = new SymTableEntry(*itOperands, -1, 0, false, 0, 'g');
									symTable.push_back(newEntry);
							}
							break;
						//.end
						case 10:
							if (currentSection > 0)
							{
								SymTableEntry* tmp = findSymTableByCount(currentSection);
								tmp->setSizeOfSection(currentLocationCounter);
							}
							end = true;
							break;
						}
					}
					else if (line->getMnemonic().find(".text") != string::npos || line->getMnemonic().find(".data") != string::npos || line->getMnemonic().find(".bss") != string::npos)
					{
						//.text/.data/.bss
						 string mnemonic = line->getMnemonic();
							if (currentSection > 0)
							{
								SymTableEntry* tmp = findSymTableByCount(currentSection);
								tmp->setSizeOfSection(currentLocationCounter);
							}
							currentSection = SymTableEntry::locationCounter;
							int rwe = 0;
							if (mnemonic.find(".text") != string::npos)
								rwe = 5;
							else if (mnemonic.find(".data") != string::npos)
								rwe = 6;
							else if (mnemonic.find(".bss") != string::npos)
								rwe = 6;

							currentLocationCounter = 0;

							SymTableEntry* n = new SymTableEntry(mnemonic, currentSection, 0, true, rwe);
							symTable.push_back(n);
							if (mnemonic.find(".text") != string::npos || mnemonic.find(".data") != string::npos)
							{
								TextTable* newTable = new TextTable(mnemonic);
								newTable->countNumber = currentSection;
								textTables.push_back(newTable);
								curSection = newTable;
							}

							if (line->existOperands())
							{
								n->setAddressInMemory(stoi(operandsVector.front()));  	//ako je nova sekcija, onda je to adresa na koju treba da se ucita u memoriju
							}
					}
					else
					{
						throw Exception("##GRESKA##     ##NEPOSTOJECA DIREKTIVA#", lineCounter, line);  //greska, nepostojeca direktiva
					}
				}
				//ako je instrukcija
				else
				{	
					if (curSection->name.find(".text") == string::npos)
						throw Exception("##GRESKA##    ##INSTRUKCIJE MORAJU BITI U TEXT SEKCIJAMA###", lineCounter, line);
					MAP::iterator it = instructionMap.find(line->getMnemonic());
					if (it != instructionMap.end())
					{
						string mnemonic = it->first;
						vector<string> operandsVector = line->getOperands();
						vector<string>::iterator itOperands = operandsVector.begin();

						switch (it->second)
						{
						case 0:
						case 1:
						case 2:
						case 3:
						case 4:
						{
							if (operandsVector.size() == 3)
							{
								//int opCode = it->second;
								//opCode <<= 26;
								INSTR4_4_18 instCode;
								instCode.opCode = it->second;
								//vector<string>::iterator itOperands = operandsVector.begin();
								MAP::iterator itReg = registerMap.find(*itOperands);
								if (itReg != registerMap.end())
									instCode.dst = itReg->second;
								itOperands++;
								itReg = registerMap.find(*itOperands);
								if (itReg != registerMap.end())
									instCode.src = itReg->second;
								itOperands++;
								int imm = stoi(*itOperands);
								instCode.imm = imm;
								curSection->insertValue(currentLocationCounter, instCode.value);
							}
							else
							{
								//greska, pogresni parametri
								throw Exception("##GRESKA##     ##POGRESAN BROJ PARAMETARA##", lineCounter, line);
							}
							break;
						}
						case 5:
						case 6:
						case 7:
						case 8:
						{
							if (operandsVector.size() == 2)
							{
								INSTR4_4_18 instCode;
								instCode.opCode = it->second;
								//vector<string>::iterator itOperands = operandsVector.begin();
								map<string, int>::iterator itReg = registerMap.find(*itOperands);
								if (itReg != registerMap.end())
									instCode.dst = itReg->second;
								itOperands++;
								itReg = registerMap.find(*itOperands);
								if (itReg != registerMap.end())
									instCode.src = itReg->second;
								instCode.imm = 0;
								curSection->insertValue(currentLocationCounter, instCode.value);
							}
							else
							{
								//greska, pogresni parametri
								throw Exception("##GRESKA##     ##POGRESAN BROJ PARAMETARA##", lineCounter, line);
							}
							break;
						}
						//sad idu ldr i str, tu verovatno mogu labele, pa neka budu posebno
						case 9:
						case 10:
						{
							if (operandsVector.size() == 3)
							{
								INSTR4_4_18 instCode;
								instCode.opCode = it->second;

								//vector<string>::iterator itOperands = operandsVector.begin();
								map<string, int>::iterator itReg = registerMap.find(*itOperands);
								if (itReg != registerMap.end())
									instCode.dst = itReg->second;
								itOperands++;
								itReg = registerMap.find(*itOperands);
								if (itReg != registerMap.end())
									instCode.src = itReg->second;
								itOperands++;
								int imm = stoi(*itOperands);
								instCode.imm = imm;
								curSection->insertValue(currentLocationCounter, instCode.value);

							}
							else
							{
								//greska, pogresni parametri
								throw Exception("##GRESKA##     ##POGRESAN BROJ PARAMETARA##", lineCounter, line);
							}
							break;
						}
						//skokovi
						case 11:
						case 12:
						case 13:
						case 14:
						case 15:
						case 16:
						case 19:
						case 20:
						case 21:
						{
							INSTR5_21 instCode;
							instCode.opCode = it->second;

							if (operandsVector.size() == 2)
							{
								//vector<string>::iterator itOperands = operandsVector.begin();
								map<string, int>::iterator itReg = registerMap.find(*itOperands);
								if (itReg != registerMap.end())
									instCode.dst = itReg->second;

								itOperands++;
								instCode.imm = stoi(*itOperands);
								curSection->insertValue(currentLocationCounter, instCode.value);
							}
							else if (operandsVector.size() == 1)
							{//u pitanju je samo labela
								instCode.dst = 16;
								//vector<string>::iterator itOperands = operandsVector.begin();
								//upisuje se 16, tj oznaka za pc

								SymTableEntry* tmp = findSymTable(*itOperands);
								if (tmp != nullptr)
								{
									//ako je def onda uzimamo vrednost, ako nije, dodajemo u forward
									if (tmp->isDefined())
									{
										if (tmp->isGlobal())
										{
											instCode.imm = -4;
											curSection->retTable->insertValue(currentLocationCounter, 2, tmp->getCount());
										}
										else
										{
											int disp = tmp->getValue() - 4;

											
											int section = tmp->getSectionNum();
											if (section != currentSection) //ako su labela i skok u istoj sekciji, ne treba ulaz za relokaciju
												curSection->retTable->insertValue(currentLocationCounter, 2, section);
											else
												disp = tmp->getValue() - (currentLocationCounter + 4);
											instCode.imm = disp;
										}

										curSection->insertValue(currentLocationCounter, instCode.value);
									}
									else
									{
										tmp->addFLink(currentLocationCounter, currentSection, 2);
										instCode.imm = -4;
										curSection->insertValue(currentLocationCounter, instCode.value);
									}
								}
								else
								{
									SymTableEntry* n = new SymTableEntry(*itOperands, 0, 0);
									n->addFLink(currentLocationCounter, currentSection, 2);
									symTable.push_back(n);
									instCode.imm = -4;
									curSection->insertValue(currentLocationCounter, instCode.value);  //ubacujemo nov element u listu, nedefinisan
								}
							}
							else
							{
								//greska, pogresni parametri
								throw Exception("##GRESKA##     ##POGRESAN BROJ PARAMETARA##", lineCounter, line);
							}
							break;
						}
						//ret/iret/jmp
						case 22:
						{
							if (operandsVector.size() == 1)
							{
								INSTR2_24 instCode;
								instCode.opCode = it->second;

								//vector<string>::iterator itOperands = operandsVector.begin();

								//string mnemonic = line->getMnemonic();
								if (mnemonic.compare("ret") == 0)
								{
									instCode.type = 0;
									instCode.imm = stoi(*itOperands);
								}
								else if (mnemonic.compare("iret") == 0)
								{
									instCode.type = 1;
									instCode.imm = stoi(*itOperands);
								}
								else if (mnemonic.compare("jmp") == 0)
								{
									if (itOperands->find("[") == string::npos)
										instCode.type = 2;
									else
									{
										instCode.type = 3;
										size_t pos = itOperands->find("]");
										*itOperands = itOperands->substr(1, pos-1);
									}
										
									///
									if (isNumeric(*itOperands))
										instCode.imm = stoi(*itOperands);
									else
									{//u pitanju je samo labela

										SymTableEntry* tmp = findSymTable(*itOperands);
										instCode.imm = -4;
										if (tmp != nullptr)
										{
											//ako je def onda uzimamo vrednost, ako nije, dodajemo u forward
											if (tmp->isDefined())
											{
												if (tmp->isGlobal())
													curSection->retTable->insertValue(currentLocationCounter, 5, tmp->getCount());
												else //lokalna
												{
													
													instCode.imm += tmp->getValue();
													int section = tmp->getSectionNum();
													if (section != currentSection) //ako su labela i skok u istoj sekciji, ne treba ulaz za relokaciju
														curSection->retTable->insertValue(currentLocationCounter, 5, section);
													else
														instCode.imm = tmp->getValue() - (currentLocationCounter + 4);

												}
											}
											else
												tmp->addFLink(currentLocationCounter, currentSection, 5);
										}
										else
										{
											SymTableEntry* n = new SymTableEntry(*itOperands, 0, 0);
											n->addFLink(currentLocationCounter, currentSection, 5);
											symTable.push_back(n);
										}
									}
								}

								curSection->insertValue(currentLocationCounter, instCode.value);
							}
							else
							{
								//greska, pogresni parametri
								throw Exception("##GRESKA##     ##POGRESAN BROJ PARAMETARA##", lineCounter, line);
							}
							break;
						}
						//push, pop
						case 23:
						case 24:
						{
							INSTR5_21 instCode;
							if (operandsVector.size() == 1)
							{
								instCode.opCode = it->second;
								//vector<string>::iterator itOperands = operandsVector.begin();
								map<string, int>::iterator itReg = registerMap.find(*itOperands);
								if (itReg != registerMap.end())
									instCode.dst = itReg->second;
								instCode.imm = 0; //not used
								curSection->insertValue(currentLocationCounter, instCode.value);
							}
							else
							{
								throw Exception("##GRESKA##     ##POGRESAN BROJ PARAMETARA##", lineCounter, line);//greska, pogresni parametri
							}
							break;
						}
						//mov, in, out
						case 25:
						case 26:
						case 27:
						{
							INSTR4_4_18 instCode;
							if (operandsVector.size() == 2)
							{
								instCode.opCode = it->second;
								instCode.imm = 0;
								//vector<string>::iterator itOperands = operandsVector.begin();
								map<string, int>::iterator itReg = registerMap.find(*itOperands);
								if (itReg != registerMap.end())
									instCode.dst = itReg->second;
								itOperands++;
								itReg = registerMap.find(*itOperands);
								if (itReg != registerMap.end())
									instCode.src = itReg->second;

								curSection->insertValue(currentLocationCounter, instCode.value);
							}
							else
							{
								throw Exception("##GRESKA##     ##POGRESAN BROJ PARAMETARA##", lineCounter, line);//greska, pogresni parametri
							}
							break;
						}
						//shr, shl
						case 28:
						case 29:
						{
							INSTR4_4_5_13 instCode;
							if (operandsVector.size() == 3)
							{
								instCode.opCode = it->second;
								map<string, int>::iterator itReg = registerMap.find(*itOperands);
								if (itReg != registerMap.end())
									instCode.dst = itReg->second;
								itOperands++;
								itReg = registerMap.find(*itOperands);
								if (itReg != registerMap.end())
									instCode.src = itReg->second;
								itOperands++;
								instCode.imm = stoi(*itOperands) << 13;
								curSection->insertValue(currentLocationCounter, instCode.value);
							}
							else
							{
								throw Exception("##GRESKA##     ##POGRESAN BROJ PARAMETARA##", lineCounter, line); //greska, pogresni parametri
							}

							break;
						}
						//int
						case 30:
						{
							INSTR4_22 instCode;
							if (operandsVector.size() == 1)
							{
								instCode.opCode = it->second;
								//vector<string>::iterator itOperands = operandsVector.begin();
								instCode.src = stoi(*itOperands);
								instCode.nu = 0; //not used
								curSection->insertValue(currentLocationCounter, instCode.value);
							}
							else
							{
								throw Exception("##GRESKA##     ##POGRESAN BROJ PARAMETARA##", lineCounter, line); //greska, pogresni parametri
							}
							break;
						}
						//ldch/l
						case 31:
						{
							if (operandsVector.size() == 2)
							{
								INSTR4_1_16_5 instCode;
								instCode.opCode = it->second;
								instCode.nu = 0; //no
								//vector<string>::iterator itOperands = operandsVector.begin();
								map<string, int>::iterator itReg = registerMap.find(*itOperands);
								if (itReg != registerMap.end())
									instCode.dst = itReg->second;
								itOperands++;
								//string mnemonic = line->getMnemonic();
								if (mnemonic.compare("ldc") == 0 || mnemonic.compare("ldch") == 0)
								{
									instCode.hl = 1;
									if (isNumeric(*itOperands))
									{
										instCode.c = stoi(*itOperands) >> 16;
									}
									else
									{//u pitanju je neka labela
										SymTableEntry* tmp = findSymTable(*itOperands);
										instCode.c = 0;
										if (tmp != nullptr)
										{
											//ako je def onda uzimamo vrednost, ako nije, dodajemo u forward
											if (tmp->isDefined())
											{
												if (tmp->isGlobal())
												{
													//curSection->insertValue(currentLocationCounter, instCode.value);
													curSection->retTable->insertValue(currentLocationCounter, 6, tmp->getCount());
												}
												else //local
												{
													instCode.c = tmp->getValue() >> 16;
													//curSection->insertValue(currentLocationCounter, instCode.value);
													curSection->retTable->insertValue(currentLocationCounter, 6, tmp->getSectionNum());
												}
											}
											else
											{
												tmp->addFLink(currentLocationCounter, currentSection, 6);
											}
										}
										else
										{
											SymTableEntry* n = new SymTableEntry(*itOperands, 0, 0);
											n->addFLink(currentLocationCounter, currentSection, 6);
											symTable.push_back(n);
											//curSection->insertValue(currentLocationCounter, opCode1);
											//ubacujemo nov element u listu, nedefinisan
										}
									}
									curSection->insertValue(currentLocationCounter, instCode.value);
									if (mnemonic.compare("ldc") == 0)
										currentLocationCounter += 4;
								}
								if (mnemonic.compare("ldc") ==0 || mnemonic.compare("ldcl") ==0)
								{
									instCode.hl = 0;
									if (isNumeric(*itOperands))
									{
										instCode.c = stoi(*itOperands);
									}
									else
									{
										//u pitanju je neka labela
										SymTableEntry* tmp = findSymTable(*itOperands);
										instCode.c = 0;
										if (tmp != nullptr)
										{
											//ako je def onda uzimamo vrednost, ako nije, dodajemo u forward
											if (tmp->isDefined())
											{
												if (tmp->isGlobal())
												{
													curSection->retTable->insertValue(currentLocationCounter, 7, tmp->getCount());
												}
												else
												{
													instCode.c = tmp->getValue();
													curSection->retTable->insertValue(currentLocationCounter, 7, tmp->getSectionNum());
												}
												//dodajemo ulaz u tabelu relokacija za tekucu sekciju
												// typeOfRelocation = 1 je za R_ABS_32
												// typeOfRelocation = 2 je za R_PC_21
												// typeOfRelocation = 3 je za R_ABS_16H
												// typeOfRelocation = 4 je za R_ABS_16L
												// typeOfRelocation = 5 je za R_PC_24
												// typeOfRelocation = 6 je za R_ABS_16H_5
												// typeOfRelocation = 7 je za R_ABS_16L_5
												/*currentLocationCounter += 4;
												LC += 4;*/
											}
											else
											{
												tmp->addFLink(currentLocationCounter, currentSection, 7);
											}

										}
										else
										{
											SymTableEntry* n = new SymTableEntry(*itOperands, 0, 0);
											n->addFLink(currentLocationCounter, currentSection, 7);
											symTable.push_back(n);
											//ubacujemo nov element u listu, nedefinisan
										}
									}
									curSection->insertValue(currentLocationCounter, instCode.value);
								}
							}
							else
							{
								throw Exception("##GRESKA##     ##POGRESAN BROJ PARAMETARA##", lineCounter, line); //greska, pogresni parametri
							}
							break;
						}
						}
						currentLocationCounter += 4;
					}
					else
					{
						throw Exception("##GRESKA##     ##NEPOSTOJECA INSTRUKCIJA#", lineCounter, line);  //greska, nepostojeca direktiva
					}
				}
			}
		}
	}
	file.close();
}

void FileManager::printFile(string s)
{
	ofstream outFile(s);
	if (outFile.is_open())
	{
		outFile << "#symTable" << '\n';
		outFile << setw(15) << left << "ime" << setw(10) << left << "sek." << setw(10) << left << "vr." << setw(10) << left << "vid." << setw(10)
			<< left << "r.b." << setw(10) << left << "vel." << setw(10) << left << "adr." << setw(10) << left << "RWE"<< '\n';

		for (list<SymTableEntry*>::iterator it = symTable.begin();it != symTable.end(); ++it)
		{
			(*it)->printIntoFile(outFile);
		}

		for (list<TextTable*>::iterator it = textTables.begin(); it != textTables.end(); ++it)
		{
			(*it)->printIntoFile(outFile);
		}
	}

	outFile.close();
}

SymTableEntry * FileManager::findSymTable(const string symbol)
{
	list<SymTableEntry*>::iterator it;
	it = symTable.begin();
	while (it != symTable.end() && (*it)->getName().compare(symbol) != 0  )
		++it;
	if (it != symTable.end())
		return *it;
	else
		return nullptr;
}

SymTableEntry * FileManager::findSymTableByCount(int count)
{
	list<SymTableEntry*>::iterator it = symTable.begin();
	while ( it != symTable.end() &&(*it)->getCount() != count )
		++it;
	if (it != symTable.end())
		return *it;
	else
		return nullptr;
}

TextTable * FileManager::findTextTable(int count)
{
	//list<TextTable*> textTables;
	list<TextTable*>::iterator it = textTables.begin();
	while (it != textTables.end() && (*it)->countNumber != count)
		++it;
	if (it != textTables.end())
		return *it;
	else
		return nullptr;

}




void FileManager::backPatch()
{

	for (list<SymTableEntry*>::iterator it = symTable.begin(); it != symTable.end(); ++it)
	{
		for (list<ForwardRefs*>::iterator fr = (*it)->fLink.begin(); fr != (*it)->fLink.end(); ++fr)
		{
			int relType = (*fr)->typeOfRelocation;
			TextTable* tt = findTextTable((*fr)->section);
			map<int, TableEntry*>::iterator te = (tt->table).find((*fr)->patch);
			switch (relType)
			{	
				//R_ABS_32//R_ABS_16H//R_ABS_16L
			case 1:
			case 3:
			case 4:
				{
						//ako je def onda uzimamo vrednost, ako nije, 
						if ((*it)->isDefined())
						{
							if ((*it)->isGlobal())
							{
								te->second->value = 0;
								tt->retTable->insertValue((*fr)->patch, relType, (*it)->getCount());
							}
							else
							{
								te->second->value = (*it)->getValue();
								tt->retTable->insertValue((*fr)->patch, relType, (*it)->getSectionNum());
							}
						}
						else
						{
							if ((*it)->section == -1)//to znaci da je extern
							{
								te->second->value = 0;
								tt->retTable->insertValue((*fr)->patch, relType, (*it)->getCount());
							}
							else
							{
								throw Exception("!!! greska, simbol nije definisan !!!\n");
							}
						}
				}
				break;
				//R_PC_21
			case 2:
			{
				INSTR5_21 instCode;
				instCode.value = te->second->value;
						if ((*it)->isDefined())
						{
							if ((*it)->isGlobal())
							{
								tt->retTable->insertValue((*fr)->patch, 2, (*it)->getCount());
							}
							else
							{
								instCode.imm += (*it)->getValue();
								int section = (*it)->getSectionNum();
								if (section != (*fr)->section) //ako su labela i skok u istoj sekciji, ne treba ulaz za relokaciju
									tt->retTable->insertValue((*fr)->patch, 2, section);
								else
									instCode.imm = (*it)->getValue() - (*fr)->patch - 4;
							}
						}
						else if ((*it)->section == -1)//to znaci da je extern
						{
							tt->retTable->insertValue((*fr)->patch,	2, (*it)->getCount());
						}
						else
						{
							throw Exception("!!! greska, simbol nije definisan !!!\n");
						}
						te->second->value = instCode.value;
			}
				break;
			case 5:
			{
				INSTR2_24 instCode;
				instCode.value = te->second->value;
				
				if ((*it)->isDefined())
				{
					if ((*it)->isGlobal())
					{
						tt->retTable->insertValue((*fr)->patch, 5, (*it)->getCount());
					}
					else
					{
						instCode.imm += (*it)->getValue();
						int section = (*it)->getSectionNum();
						if (section != (*fr)->section) //ako su labela i skok u istoj sekciji, ne treba ulaz za relokaciju
							tt->retTable->insertValue((*fr)->patch, 5, section);
						else
							instCode.imm = (*it)->getValue() - (*fr)->patch - 4;
					}
				}
				else if ((*it)->section == -1)//to znaci da je extern
				{
					tt->retTable->insertValue((*fr)->patch, 5, (*it)->getCount());
				}
				else
				{
					throw Exception("!!! greska, simbol nije definisan !!!\n");
				}
				te->second->value = instCode.value;
			}
				break;
				//R_ABS_16H_5
			case 6:
			{
				INSTR4_1_16_5 instCode;
				instCode.value = te->second->value;
				if ((*it)->isDefined())
				{
					if ((*it)->isGlobal())
					{
						tt->retTable->insertValue((*fr)->patch, 6, (*it)->getCount());
					}
					else //local
					{
						instCode.c = (*it)->getValue() >> 16;
						tt->retTable->insertValue((*fr)->patch, 6,(*it)->getSectionNum());
					}
				}
				else if ((*it)->section == -1)//to znaci da je extern
				{
					tt->retTable->insertValue((*fr)->patch, 6, (*it)->getCount());
				}
				else
				{
					throw Exception("!!! greska, simbol nije definisan !!!\n");
				}
				te->second->value = instCode.value;
			}
				break;
				//R_ABS_16L_5
			case 7:
			{
				INSTR4_1_16_5 instCode;
				instCode.value = te->second->value;
				if ((*it)->isDefined())
				{
					if ((*it)->isGlobal())
					{
						tt->retTable->insertValue((*fr)->patch, 7, (*it)->getCount());
					}
					else //local
					{
						instCode.c = (*it)->getValue();
						tt->retTable->insertValue((*fr)->patch, 7, (*it)->getSectionNum());
					}
				}
				else if ((*it)->section == -1)//to znaci da je extern
				{
					tt->retTable->insertValue((*fr)->patch, 7, (*it)->getCount());
				}
				else
				{
					throw Exception("!!! greska, simbol nije definisan !!!\n");
				}
				te->second->value = instCode.value;
			}
				break;
			}
		}
	}
}

FileManager::~FileManager()
{
	for (list<SymTableEntry*>::iterator it = symTable.begin(); it != symTable.end(); ++it)
		delete *it;
	symTable.clear();
	for (list<TextTable*>::iterator it = textTables.begin(); it != textTables.end(); ++it)
		delete *it;
	textTables.clear();
	curSection = nullptr;
}
