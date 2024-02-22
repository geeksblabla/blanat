from collections import defaultdict
from functools import reduce

class Solution:
    input_path = "input.txt"
    output_path = "output.txt"
    nb_product_to_print = 5
    
    def do(self):           
        data, total_per_city = self.load_data()
        the_cheapest_city = self.find_the_cheapest_city(total_per_city)
        output = self.get_output(the_cheapest_city, data)
        self.output_result(output, the_cheapest_city)
    
    def output_result(self, output, the_cheapest_city):
        output.insert(0, the_cheapest_city)
        output  = list(map(lambda x: f"{x[0]} {round(x[1], 2)}", output))
        txt = "\n".join(output)
        with open(self.output_path, "w") as fh:
            fh.write(txt)
        
 
    def get_output(self, the_cheapest_city, data):
        the_cheapest_city_name = the_cheapest_city[0]
        cc_data = data[the_cheapest_city_name]
        cc_data.sort(key = lambda x : x[0])
        cc_data.sort(key = lambda x:x[1])
        cc_data = cc_data[:self.nb_product_to_print]
        return cc_data
    
 
    def find_the_cheapest_city(self, total_per_city):
        cheapest_city = reduce(lambda x,y : x if x[1] <= y[1] else y, total_per_city.items())
        return cheapest_city
     
    
    def load_data(self):
        with open(self.input_path, "r") as fh:
            txt = fh.read()
        
        lines = txt.split("\n")
        default_dict_product = defaultdict(lambda : 1_000_000)
        data = defaultdict(lambda : default_dict_product)
        total_per_city = defaultdict(lambda : 0)
        
        
        for line in lines:
            if line == "":
                continue
            city, product, price = line.split(',')
            price = float(price)
            data[city][product] = price if price < data[city][product]
            total_per_city[city] += price
            
        return data, total_per_city   

if __name__ == "__main__":
    import time
    t0 = time.time()
    Solution().do()
    print("This took about", time.time() - t0)
        