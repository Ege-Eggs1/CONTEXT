# Define light-up orders for each difficulty level
Easy1 = [3, 6, 11, 14]
Easy2 = [3, 2, 4, 1, 7, 6, 8, 5, 11, 10, 12, 9, 15, 14, 16, 13]
Intermediate1 = [7, 3, 4, 2, 8, 6, 15, 11, 12, 10, 16, 14]
Advanced = [8, 5, 3, 2, 7, 6, 4, 1, 6, 7, 16, 13, 11, 10, 15, 14, 12, 9, 15, 14]

# List of all difficulty levels
difficulties = [Easy1, Easy2, Intermediate1, Advanced]
current_difficulty_index = 0  # Start with the first difficulty
Difficulty = difficulties[current_difficulty_index]  # Set the initial difficulty


# Function to handle the LED light-up sequence
def light_up_order(difficulty):
    """
    This function takes a difficulty level (list of steps) as input.
    For each step in the difficulty, it:
    - Lights up the corresponding LED in white.
    - Waits for the corresponding pressure input.
    - Changes the LED to green once the pressure is detected.
    """
    for i in difficulty:  # Iterate through each step in the difficulty list
        print(f"Lighting up LED {i} (white)")
        Light_LED(i, "white")  # Light up the current step's LED in white

        # Wait for the pressure input at the current step
        while not Pressure_input_detected(i):  # Keep checking until pressure is detected
            if button1_pressed():  # Check if the button is pressed during the sequence
                print("Button pressed! Resetting light-up sequence.")
                return  # Exit the function to reset the sequence
            print(f"Waiting for pressure on LED {i}...")
            time.sleep(0.1)  # Small delay to avoid excessive CPU usage

        # Once pressure is detected, change the LED to green
        print(f"Pressure detected on LED {i}. Changing to green.")
        Change_LED(i, "green")  # Turn LED green


# Main program loop
while True:
    """
    The main loop continuously monitors the button for difficulty change
    and runs the light-up sequence for the current difficulty.
    """
    if button1_pressed():  # Check if the button to change difficulty is pressed
        # Increment difficulty index and wrap around using modulus
        current_difficulty_index = (current_difficulty_index + 1) % len(difficulties)
        Difficulty = difficulties[current_difficulty_index]  # Update the difficulty
        print(f"Switched to difficulty {current_difficulty_index + 1}")

    # Run the light-up sequence for the current difficulty
    print("Starting light-up sequence...")
    light_up_order(Difficulty)  # Run the sequence

