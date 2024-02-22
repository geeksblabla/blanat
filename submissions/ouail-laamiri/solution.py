from pyspark.sql import SparkSession
from pyspark.sql.types import StructType, StructField, StringType, FloatType
import time

start_time = time.time()

spark = SparkSession.builder \
    .appName("Total price of products by city") \
    .getOrCreate()

schema = StructType([
    StructField("city", StringType(), True),
    StructField("product", StringType(), True),
    StructField("price", FloatType(), True)
])

# Read the CSV-like file into a DataFrame
df = spark.read.csv("./input.txt", schema=schema)

# Group by city and compute the total price of all products
total_price_by_city = df.groupBy("city").sum("price")

# Find the city with the lowest total price
city_with_lowest_price_row = total_price_by_city.orderBy("sum(price)").first()
city_with_lowest_price = city_with_lowest_price_row["city"]
total_price_in_lowest_city = city_with_lowest_price_row["sum(price)"]

# Filter the DataFrame to include only records for the city with the lowest total price
filtered_df = df.filter(df["city"] == city_with_lowest_price)

# Order the filtered DataFrame by price in ascending order and select the first five rows
five_least_expensive_products = filtered_df.orderBy("price").limit(5)

# Save the output to the file
with open("./output.txt", "w") as file:
    file.write(f"{city_with_lowest_price} {total_price_in_lowest_city}\n")
    for row in five_least_expensive_products.collect():
        file.write(f"{row['product']} {row['price']}\n")

end_time = time.time()
elapsed_time = end_time - start_time
print("Elapsed time:", elapsed_time * 1000, "ms")

spark.stop()
