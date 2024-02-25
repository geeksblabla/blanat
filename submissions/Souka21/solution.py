import pandas as pd

MAX_PRICE = 100.00
NUM_CHEAPEST_PRODUCTS = 5

def process_city_data(filename):
    """Process the input CSV file to extract city data.
    Args:
        filename (str): The name of the input CSV file.
    Returns:
        pd.DataFrame: A DataFrame containing city data.
    """
    df = pd.read_csv(filename, header=None, names=['City', 'Product', 'Price'])
    df['Price'] = df['Price'].astype(float)
    return df

def find_cheapest_city(df):
    """Find the city with the lowest total price.
    Args:
        df (pd.DataFrame): DataFrame containing city data.
    Returns:
        pd.Series: Series representing the City object with the lowest total price.
    """
    return df.groupby('City')['Price'].sum().idxmin()

def find_cheapest_products(df, min_city):
    """Find the cheapest products in a given city.
    Args:
        df (pd.DataFrame): DataFrame containing city data.
        min_city (str): Name of the city.
    Returns:
        pd.DataFrame: DataFrame containing the cheapest products in the given city.
    """
    city_df = df[df['City'] == min_city]
    return city_df.nsmallest(NUM_CHEAPEST_PRODUCTS, 'Price')[['Product', 'Price']]

def write_output(filename, min_city, min_products):
    """Write the output to a file.
    Args:
        filename (str): The name of the output file.
        min_city (str): Name of the city with the lowest total price.
        min_products (pd.DataFrame): DataFrame containing the cheapest products.
    """
    with open(filename, 'w') as file:
        file.write(f"{min_city} {min_products['Price'].sum():.2f}\n")
        for _, row in min_products.iterrows():
            file.write(f"{row['Product']} {row['Price']:.2f}\n")

def main():
    """Main function to execute the program."""
    input_filename = 'input.txt'
    output_filename = 'output.txt'

    df = process_city_data(input_filename)
    min_city = find_cheapest_city(df)
    min_products = find_cheapest_products(df, min_city)
    write_output(output_filename, min_city, min_products)

if __name__ == "__main__":
    main()
