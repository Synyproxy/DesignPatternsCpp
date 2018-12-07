#include <iostream>
#include <string>
#include <vector>

/*
 * Open-Close principle states:
 * Classes should be open for extension (by inheritance), but closed for modification.
 * Entity can allow its behaviour to be extended without modifying its source code.
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

#pragma region BadProductFilter

struct BadProductFilter																//This is functional but bad design
{
	std::vector<Product*> ByColor(std::vector<Product*> p_items, Color p_color)		//Lets say we want to filter by color
	{																				//Implementation is easy and straight forward
		std::vector<Product*> result;
		for (auto& item : p_items)
		{
			if (item->m_color == p_color)
				result.push_back(item);
		}

		return result;
	}

	std::vector<Product*> BySize(std::vector<Product*> p_items, Size p_size)		//Lets say later on we want to filter by size
	{																				//This violates the Open Close principle 
		std::vector<Product*> result;												//Because we have to change an existing class
		for (auto& item : p_items)
		{
			if (item->m_size == p_size)
				result.push_back(item);
		}

		return result;
	}

	std::vector<Product*> BySizeAndColor(std::vector<Product*> p_items,				//This also doesnt scale well
										Size p_size,								//if we have 3 criteria we would have 8 functions
										Color p_color)		
	{																						
		std::vector<Product*> result;
		for (auto& item : p_items)
		{
			if (item->m_size == p_size && item->m_color == p_color)
				result.push_back(item);
		}

		return result;
	}
};

#pragma endregion

//A better implementation that respects the Open-Close design principle. Uses the specification pattern 

#pragma region GoodProductFilter
template <typename T> struct AndSpecification;			//forward declaration

template <typename T>									//Defining a specification interface
struct ISpecification									//This will work not only with products
{
	virtual ~ISpecification() = default;
	virtual bool IsSatisfied(T* p_object) const = 0;			//pure virtual that checks if an object satisfies a specification
};

template <typename T>									//An interface for filetering
struct IFilter											//by any type of filters: ex a filter of products
{
	virtual ~IFilter() = default;
	virtual std::vector<T*> Filter(std::vector<T*> p_items,	
									ISpecification<T>& p_specification) = 0;
};

struct ProductFilter : IFilter<Product>					//Implementation of product filter
{
	std::vector<Product*> Filter(std::vector<Product*> p_products,
							ISpecification<Product>& p_specification) override
	{
		std::vector<Product*> result;
		for (auto& product : p_products)
		{
			if (p_specification.IsSatisfied(product))
				result.push_back(product);
		}

		return result;
	}
};

struct ProductColorSpecification : ISpecification<Product>	//Defining a color specification for a product by inheritance
{
	Color m_color;

	ProductColorSpecification(Color p_color): m_color(p_color){}

	bool IsSatisfied(Product* p_product) const override
	{
		return p_product->m_color == m_color;
	}
};

struct ProductSizeSpecification : ISpecification<Product>
{
	Size m_size;

	ProductSizeSpecification(Size p_size) : m_size(p_size) {}

	bool IsSatisfied(Product* p_product) const override
	{
		return p_product->m_size == m_size;
	}
};

template <typename T>
struct AndSpecification : ISpecification<T>			//combining specification
{
	const ISpecification<T>& m_first;
	const ISpecification<T>& m_second;

	AndSpecification(const ISpecification<T>& p_first,
					const ISpecification<T>& p_second)
					: m_first(p_first), m_second(p_second){}
	
	bool IsSatisfied(T* p_product) const override
	{
		return m_first.IsSatisfied(p_product) && m_second.IsSatisfied(p_product);
	}
};

template <typename T>											//To be able to combine specification with && operator
AndSpecification<T> operator&& (const ISpecification<T>& first, const ISpecification<T>& second)
{
	return AndSpecification<T>(first, second);
}

#pragma endregion

void DisplayVector(const std::vector<Product*>& p_items)		//Utility class to display results of filtering
{
	for (auto& item : p_items)
		std::cout << item->m_name << std::endl;

	std::cout << std::endl;
}

int main()
{
	Product apple {"Apple", Color::Green, Size::Small};			//Some products to test with
	Product tree {"Tree", Color::Green, Size::Large};
	Product house {"House", Color::Blue, Size::Large};

	std::vector<Product*> iventory{&apple, &tree, &house};		

	BadProductFilter badProductFilter;															//Testing the bad product filter
	auto greenThings = badProductFilter.ByColor(iventory, Color::Green);						//Functional but not good design

	std::cout << "Green Products: " << std::endl;
	DisplayVector(greenThings);

#pragma region Testing Product Filter

	ProductFilter productFilter;
	ProductColorSpecification greenProductSpecification(Color::Green);

	std::cout << "Green Products: " << std::endl;
	DisplayVector(productFilter.Filter(iventory, greenProductSpecification));

	ProductSizeSpecification largeProductSpecification(Size::Large);
	AndSpecification<Product> greenAndLargeProduct(greenProductSpecification, largeProductSpecification);

	std::cout << "Green and Large Products: " << std::endl;
	DisplayVector(productFilter.Filter(iventory, greenAndLargeProduct));

	//Compact Way
	AndSpecification<Product> compactSpecification = ProductColorSpecification(Color::Green) && ProductSizeSpecification(Size::Large);
	//auto compactSpecification = greenProductSpecification && largeProductSpecification;
	std::cout << "Green and Large Products: " << std::endl;
	DisplayVector(productFilter.Filter(iventory, compactSpecification));

#pragma endregion

	std::cin.get();
	return 0;
}