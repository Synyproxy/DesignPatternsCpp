#include <iostream>
#include <string>
#include <vector>
#include <fstream>

/*
 * The single responsibility principle states:
 * A Class Should have one primary responsibility 
 * and should not take other responsibilities 
 */

//Practical example

struct Journal														//Typical Journal with a title and entries
{
	std::string m_title;
	std::vector<std::string> m_entries;

	Journal(const std::string& p_title) : m_title(p_title) {}

	void AddEntry(const std::string& p_entry)						//Adding entries is relevant to the journal
	{
		static int entryID = 1;
		m_entries.push_back(std::to_string(entryID++) + ": " + p_entry );
		std::cout << "Entry added to jounral" << std::endl;
	}

	//This is bad
	void Save(const std::string& p_filename)						//Persistance is seperate concern from the journal
	{																//If you have other classes that need persistance
		std::ofstream ofs(p_filename);								//They need to implement their own save method
																	//If you no longer want to save to file but to a database
		for (auto& entry : m_entries)								//You will have to edit code in different classes
			ofs << entry << std::endl;

		ofs.close();
		std::cout << "Journal Saved" << std::endl;
	}
};


struct PersistanceManager											//More robust way of having persistance
{
	static void Save(const Journal& p_journal, const std::string& p_filename)								
	{																				
		std::ofstream ofs(p_filename);								//All persistance code is in one place								
																			
		for (auto& entry : p_journal.m_entries)
			ofs << entry << std::endl;

		ofs.close();
		std::cout << "Journal Saved" << std::endl;
	}
};


int main()
{
	Journal myJournal {"Game Dev Journal"};
	myJournal.AddEntry("I learned about Quaternions");
	myJournal.AddEntry("I have implemented a Quaternion class");

	//myJournal.Save("GameDevJournal.txt");						//Bad Design
	PersistanceManager pm;
	pm.Save(myJournal, "GameDevJournal.txt");

	std::cin.get();
	return 0;
}