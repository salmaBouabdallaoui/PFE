# A simple dictionary to hold the latest values for each sensor/control.
SENSOR_DATA = {
    'co2': [],               # CO2 as a graph (time-series data)
    'humidite_sol_1': [],    # Humidité du sol 1 as a graph
    'humidite_sol_2': [],    # Humidité du sol 2 as a graph
    'pompe_eau_1': 'inactive',  # Pompe 1: 'active' or 'inactive'
    'pompe_eau_2': 'inactive',  # Pompe 2: 'active' or 'inactive'
    #'temperature_1': [],     # Temperature 1 as a graph
    #'humidite_1': [],        # Humidité 1 as a graph
    'dht11_1' :[],
    'bottle_water_1': None,  # Niveau de l'eau 1 as level
    'bottle_water_2': None,  # Niveau de l'eau 2 as level
    'ultrason': None,        # Ultrason: 'suffisant' or 'insuffisante'
    'temperature_3': [],     # Temperature 3 as a graph
    'pluie': [],             # Pluie as a graph
    'flamme': None,          # Flamme: 'détecté' or 'non détecté'
    #'temperature_2': [],     # Temperature 2 as a graph
    #'humidite_2': []         # Humidité 2 as a graph
    'dht11_2':[],
}
