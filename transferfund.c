// --- NEW FUNCTION: TRANSFER FUNDS ---
void transferFunds(FILE *fPtr) {
    unsigned int fromAcct, toAcct;
    double amount;
    struct clientData source = {0, "", "", 0.0};
    struct clientData destination = {0, "", "", 0.0};

    printf("Enter account number to transfer FROM: ");
    scanf("%u", &fromAcct);
    printf("Enter account number to transfer TO: ");
    scanf("%u", &toAcct);
    printf("Enter amount to transfer: ");
    scanf("%lf", &amount);

    // Read Source Account
    fseek(fPtr, (fromAcct - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&source, sizeof(struct clientData), 1, fPtr);

    // Read Destination Account
    fseek(fPtr, (toAcct - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&destination, sizeof(struct clientData), 1, fPtr);

    // Validation
    if (source.acctNum == 0 || destination.acctNum == 0) {
        printf("Error: One or both accounts do not exist.\n");
    } else if (amount <= 0) {
        printf("Error: Transfer amount must be positive.\n");
    } else if (source.balance < amount) {
        printf("Transfer denied! Account #%u has insufficient funds (Balance: %.2f).\n", fromAcct, source.balance);
    } else {
        // Update balances in memory
        source.balance -= amount;
        destination.balance += amount;

        // Write Source back to file
        fseek(fPtr, (fromAcct - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&source, sizeof(struct clientData), 1, fPtr);

        // Write Destination back to file
        fseek(fPtr, (toAcct - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&destination, sizeof(struct clientData), 1, fPtr);

        printf("Successfully transferred $%.2f from #%u to #%u.\n", amount, fromAcct, toAcct);
    }
}