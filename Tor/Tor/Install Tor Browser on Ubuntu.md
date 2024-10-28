# step-by-step instructions on how to install the Tor Browser and connect to the Tor network on Ubuntu:

### Install Tor Browser on Ubuntu

1. **Download Tor Browser:**
   - Open a terminal by pressing `Ctrl + Alt + T`.
   - Run the following commands to download the Tor Browser:
     ```bash
     wget https://www.torproject.org/dist/torbrowser/11.0.1/tor-browser-linux64-11.0.1_en-US.tar.xz
     ```

2. **Extract the Tarball:**
   - Run the following command to extract the downloaded tarball:
     ```bash
     tar -xvf tor-browser-linux64-11.0.1_en-US.tar.xz
     ```

3. **Navigate to the Tor Browser Directory:**
   - Move to the extracted directory using:
     ```bash
     cd tor-browser_en-US
     ```

4. **Run the Tor Browser:**
   - Start the Tor Browser by running:
     ```bash
     ./start-tor-browser.desktop
     ```

### Connecting to the Tor Network

1. **Launch Tor Browser:**
   - Open the Tor Browser using the method mentioned in the previous step.

2. **Connect to the Tor Network:**
   - Once the Tor Browser is open, you will see a connection screen. Click on "Connect" to establish a connection to the Tor network.

3. **Wait for Connection:**
   - The Tor Browser will take a moment to connect to the Tor network. Once connected, you can start using the browser.

### Verify Connection:

To verify that you are connected to the Tor network, you can visit the following website in the Tor Browser: [https://check.torproject.org/](https://check.torproject.org/). This page will confirm whether your browser is configured to use Tor.

Now you have successfully installed the Tor Browser and connected to the Tor network on your Ubuntu system. Remember to only use the Tor network for activities that require anonymity and privacy.
