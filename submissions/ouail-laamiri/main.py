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
df = spark.read.csv("input.txt", schema=schema)

# Group by city and compute the total price of all products
total_price_by_city = df.groupBy("city").sum("price")

# Show the total price of all products by city
total_price_by_city.show()

# Record the end time
end_time = time.time()

# Calculate the elapsed time
elapsed_time = end_time - start_time
print("Elapsed time:", elapsed_time, "seconds")

# Stop the SparkSession
spark.stop()
