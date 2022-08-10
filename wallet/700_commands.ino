//========================================================================//
//================================COMMANDS================================//
//========================================================================//

String encrytptedMnemonic = "";
String passwordHash = "";

bool authenticated = false;
String command = ""; // todo: remove
String commandData = ""; // todo: remove

String message = "Welcome";
String subMessage = "Enter password";

String serialData = "";

void listenForCommands() {
  // todo: called too many times
  if (loadFiles() == false) {
    message = "Failed opening files";
    subMessage = "Reset or 'help'";
  }

  if (message != "" || subMessage != "")
    showMessage(message, subMessage);

  serialData = awaitSerialData();

  Command c = extractCommand(serialData);
  // flush stale data from buffer
  Serial.println("Received command: " + c.cmd);
  executeCommand(c);

  delay(DELAY_MS);
}


void executeCommand(Command c) {
  if (c.cmd == COMMAND_HELP) {
    executeHelp(c.data);
  } else if (c.cmd == COMMAND_WIPE) {
    executeWipeHww(c.data);
  } else if (c.cmd == COMMAND_PASSWORD) {
    executePasswordCheck(c.data);
  } else if (c.cmd == COMMAND_PASSWORD_CLEAR) {
    executePasswordClear(c.data);
  } else if (c.cmd == COMMAND_SEED) {
    executeShowSeed(c.data);
  } else if (c.cmd == COMMAND_SEND_PSBT) {
    executeSignPsbt(c.data);
  } else if (c.cmd == COMMAND_RESTORE) {
    executeRestore(c.data, "");
  } else if (c.cmd == COMMAND_XPUB) {
    executeXpub(c.data);
  } else {
    executeUnknown(c.data);
  }
}
void executeHelp(String commandData) {
  // help();
  Serial.println("### executeHelp 1: " + commandData);
  String messageB = "Hello HWW  World";
  byte plainB[messageB.length()];
  messageB.getBytes(plainB, sizeof(messageB));
  Serial.println("### messageB: " + toHex(plainB, sizeof(plainB)));
  
  String alicesPrivateKeyHex =  "359a8ca1418c49dd26dc7d92c789ac33347f64c6b7789c666098805af3cc60e5";
  String bobsPrivateKeyHex = "ab52f1f981f639bd83f884703bc690b10db709ff48806680a0d3fbc6475e6093";


  uint8_t alicePrivateKeyBin[32];
  int len = fromHex(alicesPrivateKeyHex, alicePrivateKeyBin, 32);
  PrivateKey alicePrivateKey(alicePrivateKeyBin);
  PublicKey alicePublicKey = alicePrivateKey.publicKey();

  uint8_t bobsPrivateKeyBin[32];
  fromHex(bobsPrivateKeyHex, bobsPrivateKeyBin, 32);
  PrivateKey bobsPrivateKey(bobsPrivateKeyBin);
  PublicKey bobsPublicKey = bobsPrivateKey.publicKey();

  byte shared_secret1[32];
  bobsPrivateKey.ecdh(alicePublicKey, shared_secret1, false);
  Serial.println("### shared_secret1: " + toHex(shared_secret1, sizeof(shared_secret1)));
  


  String messageHex = commandData;
  int byteSize =  messageHex.length() / 2;
  byte messageBin[byteSize];

  Serial.println("### messageHex: " + messageHex);
  Serial.println("### byteSize: "+ String(byteSize));

  fromHex(messageHex, messageBin, byteSize);
  Serial.println("### messageText return0: " + toHex(messageBin, sizeof(messageBin)));

  uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
  byte shared_secret[32];
  fromHex("f96c85875055a5586688fea4cf7c4a2bd9541ffcf34f9d663d97e0cf2f6af4af", shared_secret, 32);
  Serial.println("### iv: " + toHex(iv, sizeof(iv)));
  
  AES_ctx ctx;
  AES_init_ctx_iv(&ctx, shared_secret, iv);
  AES_CBC_decrypt_buffer(&ctx, messageBin, sizeof(messageBin));

  String commandTxt = String((char *)messageBin);
  Serial.println("### messageText return1: " + commandTxt.substring(0, byteSize));
  Serial.println("### messageText return2: " + commandTxt);
  Serial.println("### messageText return3: " + toHex(messageBin, sizeof(messageBin)));
  
  Serial.println("### end help 1");
}

/*
void executeHelpTest(String commandData) {
  // help();
  Serial.println("### executeHelp: " + commandData);
  String alicesPrivateKeyHex =  "359a8ca1418c49dd26dc7d92c789ac33347f64c6b7789c666098805af3cc60e5";
  String bobsPrivateKeyHex = "ab52f1f981f639bd83f884703bc690b10db709ff48806680a0d3fbc6475e6093";


  uint8_t alicePrivateKeyBin[32];
  int len = fromHex(alicesPrivateKeyHex, alicePrivateKeyBin, 32);
  PrivateKey alicePrivateKey(alicePrivateKeyBin);
  PublicKey alicePublicKey = alicePrivateKey.publicKey();

  // Serial.println("### alice length: " + String(len));
  // Serial.println("### alicePrivateKey: " + toHex(alicePrivateKeyBin, len));
  // Serial.println("### alicePublicKey: " + toHex(alicePublicKey.point, 64));

  uint8_t bobsPrivateKeyBin[32];
  fromHex(bobsPrivateKeyHex, bobsPrivateKeyBin, 32);
  PrivateKey bobsPrivateKey(bobsPrivateKeyBin);
  PublicKey bobsPublicKey = bobsPrivateKey.publicKey();

  // Serial.println("### length: " + String(len));
  // Serial.println("### bobsPrivateKey: " + toHex(bobsPrivateKeyBin, len));
  // Serial.println("### bobsPublicKey: " + toHex(bobsPublicKey.point, 64));


  byte shared_secret1[32];
  bobsPrivateKey.ecdh(alicePublicKey, shared_secret1, false);
  Serial.println("### shared_secret1: " + toHex(shared_secret1, sizeof(shared_secret1)));

  byte shared_secret2[32];
  alicePrivateKey.ecdh(bobsPublicKey, shared_secret2, false);
  // Serial.println("### shared_secret2: " + toHex(shared_secret2, sizeof(shared_secret2)));






  uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
  uint8_t in[]  = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
                    0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
                    0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
                    0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
                  };

  Serial.println("### iv: " + toHex(iv, sizeof(iv)));

  Serial.println("###");
  Serial.println("### in: " + toHex(in, sizeof(in)));

  AES_ctx ctx;
  AES_init_ctx_iv(&ctx, shared_secret1, iv);

  AES_CBC_encrypt_buffer(&ctx, in, 64);
  Serial.println("### in2: " + toHex(in, sizeof(in)));

  AES_init_ctx_iv(&ctx, shared_secret1, iv);
  AES_CBC_decrypt_buffer(&ctx, in, 64);
  Serial.println("### in3: " + toHex(in, sizeof(in)));

  String message = "Hello ₿ World";
  byte plain[message.length()];
  message.getBytes(plain, sizeof(message));
  Serial.println("### message2: " + String((char *)plain));

  String messageHex = commandData; //toHex(plain, sizeof(plain));
  Serial.println("### messageHex: " + messageHex);
  int byteSize =  messageHex.length() / 2; // sizeof(messageHex); 
  Serial.println("### byteSize: "+ String(byteSize));
  byte messageBin[byteSize];
  fromHex(messageHex, messageBin, byteSize);
  // Serial.println("### messageHex return: " + toHex(messageBin, sizeof(messageBin)));
  String commandTxt = (char *)messageBin;
  Serial.println("### messageText return: " + commandTxt.substring(0, byteSize));

  // AES_init_ctx_iv(&ctx, shared_secret1, iv);
  // AES_CBC_decrypt_buffer(&ctx, messageBin, byteSize);
  // Serial.println("### re-message decrypted: " + toHex(messageBin, sizeof(messageBin)));
  Serial.println("### end help");
}
*/

void executePasswordCheck(String commandData) {
  if (commandData == "") {
    message = "Enter password";
    subMessage = "8 numbers/letters";
    return;
  }
  String hash = hashPassword(commandData);
  if (passwordHash == hash) {
    authenticated = true;
    message = "Password correct!";
    subMessage = "Ready to sign sir!";
  } else {
    authenticated = false;
    message = "Wrong password, try again";
    subMessage = "8 numbers/letters";
  }
  Serial.println(COMMAND_PASSWORD + " " + String(authenticated));
}

void executePasswordClear(String commandData) {
  authenticated = false;
  Serial.println(COMMAND_PASSWORD_CLEAR + " 1");
  showMessage("Logging out...", "");
  delay(2000);

  message = "Logged out";
  subMessage = "Enter password";
}

void executeWipeHww(String password) {
  if (password == "") {
    message = "Enter new password";
    subMessage = "8 numbers/letters";
    return;
  }

  showMessage("Resetting...", "");
  delay(2000);

  authenticated = wipeHww(password, "");
  if (authenticated == true) {
    message = "Successfully wiped!";
    subMessage = "Every new beginning comes from some other beginning's end.";
  } else {
    message = "Error, try again";
    subMessage = "8 numbers/letters";
  }
  Serial.println(COMMAND_WIPE + " " + String(authenticated));
}

void executeShowSeed(String position) {
  if (authenticated == false) {
    message = "Enter password!";
    subMessage = "8 numbers/letters";
    return;
  }
  if (!position || position.toInt() == 0) {
    message = "Bad word position";
    subMessage = "`/help` for details ";
    return;
  }
  message = "";
  subMessage = "";
  String word = getWordAtPosition(encrytptedMnemonic, position.toInt());
  printMnemonicWord(position, word);
}

void executeXpub(String commandData) {
  if (authenticated == false) {
    message = "Enter password!";
    subMessage = "8 numbers/letters";
    return;
  }

  int spacePos = commandData.indexOf(" ");
  String networkName = commandData.substring(0, spacePos);
  String path = commandData.substring(spacePos + 1, commandData.length() );

  Serial.println("xpub received: " + networkName + " path:" + path);

  const Network * network;
  if (networkName == "Mainnet") {
    network = &Mainnet;
  } else if (networkName == "Testnet") {
    network = &Testnet;
  } else {
    message = "Unknown Network";
    subMessage = "Must be Mainent or Testnet";
    return;
  }

  if (!path) {
    message = "Derivation path missing!";
    subMessage = "XPUB not generated";
    return;
  }

  HDPrivateKey hd(encrytptedMnemonic, "", &Testnet);
  if (!hd) {
    message = "Invalid Mnemonic";
    Serial.println(COMMAND_XPUB + " 0 invalid_mnemonic");
    return;
  }
  HDPrivateKey account = hd.derive(path);
  String xpub = account.xpub();
  Serial.println(COMMAND_XPUB + " 1 " + xpub + " " + hd.fingerprint());
  message = xpub;
}

void executeRestore(String mnemonic, String password) {
  if (mnemonic == "") {
    message = "Enter seed words";
    subMessage = "Separated by spaces";
    return;
  }

  int size = getMnemonicBytes(mnemonic);
  if (size == 0) {
    message = "Wrong word count!";
    subMessage = "Must be 12, 15, 18, 21 or 24";
    Serial.println(COMMAND_RESTORE + " 0");
    return;
  }

  if (!hasValidChecksum(mnemonic, size)) {
    message = "Wrong mnemonic!";
    subMessage = "Incorrect checksum";
    Serial.println(COMMAND_RESTORE + " 0");
    return;
  }

  if (password == "") {
    showMessage("Enter new password!", "8 numbers/letters");
    serialData = awaitSerialData();
    Command c = extractCommand(serialData);
    if (c.cmd != COMMAND_PASSWORD) {
      executeUnknown("");
      return;
    }
    password = c.data;
  }

  authenticated = wipeHww(password, mnemonic);
  if (authenticated == true) {
    message = "Restore successfull";
    subMessage = "/seed` to view word list";
  } else {
    message = "Error, try again";
    subMessage = "8 numbers/letters";
  }
  Serial.println(COMMAND_RESTORE + " " + String(authenticated));
}

void executeSignPsbt(String commandData) {
  if (authenticated == false) {
    message = "Enter password!";
    subMessage = "8 numbers/letters";
    return;
  }

  showMessage("Please wait", "Parsing PSBT...");
  int spacePos = commandData.indexOf(" ");
  String networkName = commandData.substring(0, spacePos);
  String psbtBase64 = commandData.substring(spacePos + 1, commandData.length() );

  const Network * network;
  if (networkName == "Mainnet") {
    network = &Mainnet;
  } else if (networkName == "Testnet") {
    network = &Testnet;
  } else {
    message = "Unknown Network";
    subMessage = "Must be Mainent or Testnet";
    return;
  }

  PSBT psbt = parseBase64Psbt(psbtBase64);
  if (!psbt) {
    Serial.println("Failed psbt: " + psbtBase64);
    message = "Failed parsing";
    subMessage = "Send PSBT again";
    Serial.println(COMMAND_SEND_PSBT + " psbt_parse_failed");
    return;
  }

  HDPrivateKey hd(encrytptedMnemonic, "", network); // todo: no passphrase yet
  // check if it is valid
  if (!hd) {
    message = "Invalid Mnemonic";
    Serial.println(COMMAND_SEND_PSBT + " invalid_mnemonic'");
    return;
  }

  Serial.println(COMMAND_SEND_PSBT + " 1");

  for (int i = 0; i < psbt.tx.outputsNumber; i++) {
    printOutputDetails(psbt, hd, i, network);
    serialData = awaitSerialData();
    Command c = extractCommand(serialData);
    if (c.cmd == COMMAND_CANCEL) {
      message = "Operation Canceled";
      subMessage = "`/help` for details";
      return;
    } else if (c.cmd != COMMAND_CONFIRM_NEXT) {
      executeUnknown(c.data);
      return;
    }
  }
  printFeeDetails(psbt.fee());


  serialData = awaitSerialData();
  Command c = extractCommand(serialData);
  if (c.cmd == COMMAND_SIGN_PSBT) {
    showMessage("Please wait", "Signing PSBT...");

    uint8_t signedInputCount = psbt.sign(hd);

    Serial.println(COMMAND_SIGN_PSBT + " " + signedInputCount + " " + psbt.toBase64());
    message = "Signed inputs:";
    subMessage = String(signedInputCount);
  } else if (c.cmd = COMMAND_CANCEL) {
    message = "Operation Canceled";
    subMessage = "`/help` for details";
  } else {
    executeUnknown(commandData);
  }
}

void executeUnknown(String commandData) {
  message = "Unknown command";
  subMessage = "`/help` for details";
}


bool loadFiles() {
  FileData mnFile = readFile(SPIFFS, "/mn.txt");
  encrytptedMnemonic = mnFile.data;

  FileData pwdFile = readFile(SPIFFS, "/hash.txt");
  passwordHash = pwdFile.data;

  return mnFile.success && pwdFile.success;
}

bool wipeHww(String password, String mnemonic) {
  if (isAlphaNumeric(password) == false)
    return false;

  deleteFile(SPIFFS, "/mn.txt");
  deleteFile(SPIFFS, "/hash.txt");
  if (mnemonic == "") {
    mnemonic = createMnemonic(24); // todo: allow 12 also
  }
  passwordHash = hashPassword(password);
  writeFile(SPIFFS, "/mn.txt", mnemonic);
  writeFile(SPIFFS, "/hash.txt", passwordHash);

  delay(DELAY_MS);
  return true;
}


bool hasValidChecksum(String mnemonic, int size) {
  uint8_t out[size];
  size_t len = mnemonicToEntropy(mnemonic, out, sizeof(out));
  String deserializedMnemonic = mnemonicFromEntropy(out, sizeof(out));
  Serial.println("mnemonic: " + mnemonic);
  Serial.println("deserializedMnemonic: " + deserializedMnemonic);
  Serial.println("mnemonic == deserializedMnemonic: " + String(mnemonic == deserializedMnemonic));
  return mnemonic == deserializedMnemonic;
}


Command extractCommand(String s) {
  int spacePos = s.indexOf(" ");
  command = s.substring(0, spacePos);
  if (spacePos == -1) {
    commandData = "";
  } else {
    commandData = s.substring(spacePos + 1, s.length());
  }
  return {command, commandData};
}
