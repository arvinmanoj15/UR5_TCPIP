#include <iostream>
#include <modbus.h>

int main() {
    modbus_t *ctx;
    uint8_t coil_value;
    uint8_t flag = 0;
    
    // Create a new Modbus context
    ctx = modbus_new_tcp("192.168.131.40", 502); //ridgeback UR5 IP
    if (ctx == NULL) {
        std::cerr << "Unable to create the Modbus context." << std::endl;
        return 1;
    }
    
    // Connect to the Modbus server
    if (modbus_connect(ctx) == -1) {
        std::cerr << "Modbus connection failed: " << modbus_strerror(errno) << std::endl;
        modbus_free(ctx);
        return 1;
    }
    
    // Read the coil value at address 261
    while (modbus_read_bits(ctx, 261, 1, &coil_value) != -1) {
        
        // Print the coil value
    	//std::cout << "Coil value at address 261: " << static_cast<int>(coil_value) << std::endl;
    	if (static_cast<int>(coil_value) == 1 && flag == 0)
    	{
    		std::cout << "UR5 STOPPED!" << std::endl;
    		flag = 1;
    		// Disconnect and free the Modbus context
	    	//modbus_close(ctx);
	    	//modbus_free(ctx);
	        //return 0;
    	}
    	else if (static_cast<int>(coil_value) == 0 && flag == 1)
    	{
    		flag = 0;
    	}
        
    }
    
    
    
    std::cerr << "Modbus read failed: " << modbus_strerror(errno) << std::endl;
    modbus_close(ctx);
    modbus_free(ctx);
    return 1;
    
    
}

