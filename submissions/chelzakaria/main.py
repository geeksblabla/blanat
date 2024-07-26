import polars as pl


def fun():
    df = pl.scan_csv("input.txt", new_columns=["City", "Product", "Price"])

    cheapest_city = (
        df.group_by("City")
        .agg(pl.sum("Price").alias("Total_Price"))
        .sort("Total_Price")
        .select(["City", "Total_Price"])
        .collect(streaming=True)
        .head(1)
    )

    cheapest_city_df = df.filter(pl.col("City") == cheapest_city["City"][0])
    result = (
        cheapest_city_df.group_by("Product")
        .agg(pl.min("Price").alias("Min_Price"))
        .sort(["Min_Price", "Product"])
        .select(["Product", "Min_Price"])
        .limit(5)
        .collect(streaming=True)
    )

    with open("output.txt", "w") as f:
        cheapest_city.write_csv(f, has_header=False, separator=" ")
        result.write_csv(f, has_header=False, separator=" ")


if __name__ == "__main__":
    fun()
