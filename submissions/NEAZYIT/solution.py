# Import necessary libraries
import dask.dataframe as dd
import logging
import sys
import pandas as pd

# Configure logging to write to console and a file
logging.basicConfig(level=logging.INFO,
                    format='%(levelname)s: %(message)s',
                    handlers=[
                        logging.StreamHandler(
                            stream=sys.stdout),  # Log to console
                        # Log to file with 'utf-8' encoding
                        logging.FileHandler('app.log', encoding='utf-8')
                    ])


def read_data(input_file):
    """
    📖 Read data from a CSV file using Dask.

    Parameters:
    - input_file (str): The path to the input CSV file.

    Returns:
    - dd.DataFrame: A Dask DataFrame containing the read data.
    """
    try:
        # Open the file and inspect its content for decoding
        with open(input_file, 'rb') as f:
            raw_content = f.read()

        # Decode the content using 'utf-8' and 'ignore' errors
        decoded_content = raw_content.decode('utf-8', errors='ignore')

        # Create a table structure for easy analysis
        from io import StringIO
        pandas_df = pd.read_csv(StringIO(decoded_content), header=None, names=[
                                'City', 'Product', 'Price'])
        # Convert the table into a format suitable for large datasets
        dask_df = dd.from_pandas(pandas_df, npartitions=1)

        return dask_df
    except pd.errors.EmptyDataError:
        logging.error(
            f"❌ Error reading data from {input_file}: The file is empty.")
        raise
    except pd.errors.ParserError as pe:
        logging.error(f"❌ Error reading data from {input_file}: {pe}")
        raise
    except Exception as e:
        logging.error(
            f"❌ An unexpected error occurred while reading data from {input_file}: {e}")
        raise


def validate_prices(data):
    """
    ✅ Validate prices to be within the specified range.

    Parameters:
    - data (dd.DataFrame): Dask DataFrame containing the data.

    Returns:
    - dd.DataFrame: Filtered Dask DataFrame with valid prices.
    """
    try:
        # Filter out unreasonable prices
        valid_price_range = (data['Price'] >= 1.00) & (data['Price'] <= 100.00)
        return data[valid_price_range]
    except Exception as e:
        logging.error(f"❌ Error validating prices: {e}")
        raise


def clean_data(data):
    """
    🧹 Clean data by removing missing values and duplicates.

    Parameters:
    - data (dd.DataFrame): Dask DataFrame containing the data.

    Returns:
    - dd.DataFrame: Cleaned data without unnecessary information.
    """
    try:
        # Remove any missing values or duplicate entries
        data = data.dropna()
        return data.drop_duplicates()
    except Exception as e:
        logging.error(f"❌ Error cleaning data: {e}")
        raise


def group_and_aggregate(data):
    """
    📊 Group and aggregate data to find total prices for each city.

    Parameters:
    - data (dd.DataFrame): Dask DataFrame containing the data.

    Returns:
    - dd.Series: Summary of total prices for each city.
    """
    try:
        return data.groupby('City')['Price'].sum().compute()
    except Exception as e:
        logging.error(f"❌ Error grouping and aggregating data: {e}")
        raise


def sort_and_select_top_products(data, cheapest_city):
    """
    🔄 Sort the data for the cheapest city and select the top 5 products.

    Parameters:
    - data (dd.DataFrame): Dask DataFrame containing the data.
    - cheapest_city (str): The city with the lowest total price.

    Returns:
    - dd.DataFrame: Sorted Dask DataFrame with the top 5 products.
    """
    try:
        # Filter data for the cheapest city, sort, and directly select top 5 rows
        return data[data['City'] == cheapest_city].compute().sort_values(['Price', 'Product']).drop_duplicates('Product').head(5)
    except Exception as e:
        logging.error(f"❌ Error sorting and selecting top products: {e}")
        raise


def write_results(output_file, cheapest_city, total_price, sorted_products):
    """
    📝 Write results to an output file.

    Parameters:
    - output_file (str): The path to the output file.
    - cheapest_city (str): The city with the lowest total price.
    - total_price (float): The total price for the cheapest city.
    - sorted_products (dd.DataFrame): Dask DataFrame containing sorted products.

    Returns:
    - None
    """
    try:
        with open(output_file, 'w') as f:
            # First line: Cheapest city and total price
            print(f"{cheapest_city} {total_price:.2f}", file=f)

            # Remaining lines: Top 5 cheapest products
            for _, row in sorted_products.iterrows():
                print(f"{row['Product']} {row['Price']:.2f}", file=f)

        logging.info(f"✅ Results written to {output_file}")
    except Exception as e:
        logging.error(f"❌ Error writing results to {output_file}: {e}")
        raise


def process_data(input_file, output_file):
    """
    🔄 Process data from an input file and write results to an output file.

    Parameters:
    - input_file (str): The path to the input CSV file.
    - output_file (str): The path to the output file.
    """
    try:
        # Step 1: Read data
        data = read_data(input_file)

        # Step 2: Validate prices
        data = validate_prices(data)

        # Step 3: Clean data
        data = clean_data(data)

        # Step 4: Group and aggregate
        city_total_prices = group_and_aggregate(data)

        # Step 5: Find the cheapest city
        cheapest_city = city_total_prices.idxmin()

        # Step 6: Sort and select top products for the cheapest city
        sorted_products = sort_and_select_top_products(data, cheapest_city)

        # Step 7: Calculate the total price for the cheapest city
        total_price = sorted_products['Price'].sum()

        # Step 8: Write results to the output file
        write_results(output_file, cheapest_city, total_price, sorted_products)

        logging.info("✅ Data processing completed successfully!")

    except Exception as e:
        logging.error(f"❌ An error occurred during data processing: {e}")
        raise


if __name__ == "__main__":
    process_data("input.txt", "./output.txt")