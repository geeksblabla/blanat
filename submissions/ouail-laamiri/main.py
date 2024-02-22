from pyspark.sql import SparkSession
from pyspark.sql.types import StructType, StructField, StringType, FloatType
import time

# Record the start time
start_time = time.time()

# Create a SparkSession
spark = SparkSession.builder \
    .appName("Total price of products by city") \
    .getOrCreate()

# Define the schema for the DataFrame
schema = StructType([
    StructField("city", StringType(), True),
    StructField("product", StringType(), True),
    StructField("price", FloatType(), True)
])

# Read the CSV-like file into a DataFrame
df = spark.read.csv("../../input.txt", schema=schema)

# Group by city and compute the total price of all products
total_price_by_city = df.groupBy("city").sum("price")

# Find the city with the lowest total price
city_with_lowest_price = total_price_by_city.orderBy("sum(price)").first()["city"]

# Filter the DataFrame to include only records for the city with the lowest total price
filtered_df = df.filter(df["city"] == city_with_lowest_price)

# Retrieve the total price for the city with the lowest total product price
total_price_in_lowest_city = total_price_by_city.filter(total_price_by_city["city"] == city_with_lowest_price).collect()[0][1]

# Order the filtered DataFrame by price in ascending order and select the first five rows
five_least_expensive_products = filtered_df.orderBy("price").limit(5)

# Show the five least expensive products in the city with the lowest total price
print("Five least expensive products in", city_with_lowest_price , "total price : ",total_price_in_lowest_city)
five_least_expensive_products.show()


# Record the end time
end_time = time.time()

# Calculate the elapsed time
elapsed_time = end_time - start_time
print("Elapsed time:", elapsed_time, "seconds")

# Stop the SparkSession
spark.stop()
