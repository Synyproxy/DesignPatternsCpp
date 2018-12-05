#include <iostream>
#include <string>
#include <vector>

/*
 * Open-Close principle states:
 *
 * 
 */

//Pratical Example

/*
 * Imagine you have Products that you want to filter with critirea 
 */

enum class Color {Red, Green, Blue};
enum class Size {Small, Medium, Large};

struct Product
{
	std::string m_name;
	Color m_color;
	Size m_size;
};

struct ProductFilter
{
	std::vector<Product*> by_color(std::vector<Product*> p_items, Color p_color)
	{
		std::vector<Product*> result;
		for (auto& item : p_items)
		{
			if (item->m_color == p_color)
				result.push_back(item);

			return result;
		}
	}
};

int main()
{
	Product apple {"Apple", Color::Green, Size::Small};
	Product tree {"Tree", Color::Green, Size::Large};
	Product house {"House", Color::Blue, Size::Large};


	std::cout << "Hello" << std::endl;
	return 0;
}