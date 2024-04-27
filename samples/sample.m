// AI generated sample

#import <Foundation/Foundation.h>

// Customer class
@interface Customer : NSObject
@property (strong, nonatomic) NSString *name;
@property (strong, nonatomic) NSString *address;
- (instancetype)initWithName:(NSString *)name address:(NSString *)address;
@end

@implementation Customer
- (instancetype)initWithName:(NSString *)name address:(NSString *)address {
    self = [super init];
    if (self) {
        _name = name;
        _address = address;
    }
    return self;
}
@end

// Account class
@interface Account : NSObject
@property (assign, nonatomic) NSInteger accountNumber;
@property (strong, nonatomic) Customer *customer;
@property (assign, nonatomic) double balance;
+ (NSInteger)accountNumberCounter;
- (instancetype)initWithCustomer:(Customer *)customer initialDeposit:(double)deposit;
- (void)deposit:(double)amount;
- (void)withdraw:(double)amount;
- (void)displayDetails;
@end

@implementation Account
static NSInteger _accountNumberCounter = 1000;

+ (NSInteger)accountNumberCounter {
    return _accountNumberCounter;
}

- (instancetype)initWithCustomer:(Customer *)customer initialDeposit:(double)deposit {
    self = [super init];
    if (self) {
        _customer = customer;
        _balance = deposit;
        _accountNumber = ++_accountNumberCounter;
    }
    return self;
}

- (void)deposit:(double)amount {
    _balance += amount;
    NSLog(@"Deposited %.2f. New balance: %.2f", amount, _balance);
}

- (void)withdraw:(double)amount {
    if (amount > _balance) {
        NSLog(@"Insufficient funds.");
    } else {
        _balance -= amount;
        NSLog(@"Withdrew %.2f. New balance: %.2f", amount, _balance);
    }
}

- (void)displayDetails {
    NSLog(@"Account Number: %ld", (long)_accountNumber);
    NSLog(@"Customer Name: %@", _customer.name);
    NSLog(@"Account Balance: %.2f", _balance);
}
@end

// Bank class
@interface Bank : NSObject
@property (strong, nonatomic) NSString *name;
@property (strong, nonatomic) NSMutableDictionary<NSNumber *, Account *> *accounts;
- (instancetype)initWithName:(NSString *)name;
- (NSNumber *)createAccountForCustomer:(Customer *)customer withInitialDeposit:(double)deposit;
- (void)deposit:(double)amount intoAccount:(NSNumber *)accountNumber;
- (void)withdraw:(double)amount fromAccount:(NSNumber *)accountNumber;
- (void)displayDetailsForAccount:(NSNumber *)accountNumber;
@end

@implementation Bank
- (instancetype)initWithName:(NSString *)name {
    self = [super init];
    if (self) {
        _name = name;
        _accounts = [[NSMutableDictionary alloc] init];
    }
    return self;
}

- (NSNumber *)createAccountForCustomer:(Customer *)customer withInitialDeposit:(double)deposit {
    Account *account = [[Account alloc] initWithCustomer:customer initialDeposit:deposit];
    _accounts[@(account.accountNumber)] = account;
    return @(account.accountNumber);
}

- (void)deposit:(double)amount intoAccount:(NSNumber *)accountNumber {
    Account *account = _accounts[accountNumber];
    if (account) {
        [account deposit:amount];
    } else {
        NSLog(@"Account not found.");
    }
}

- (void)withdraw:(double)amount fromAccount:(NSNumber *)accountNumber {
    Account *account = _accounts[accountNumber];
    if (account) {
        [account withdraw:amount];
    } else {
        NSLog(@"Account not found.");
    }
}

- (void)displayDetailsForAccount:(NSNumber *)accountNumber {
    Account *account = _accounts[accountNumber];
    if (account) {
        [account displayDetails];
    } else {
        NSLog(@"Account not found.");
    }
}
@end

// Main
int main(int argc, const char * argv[]) {
    @autoreleasepool {
        Bank *bank = [[Bank alloc] initWithName:@"Copilot Bank"];
        
        // Create customers
        Customer *john = [[Customer alloc] initWithName:@"John Doe" address:@"123 Elm Street"];
        Customer *jane = [[Customer alloc] initWithName:@"Jane Doe" address:@"456 Oak Street"];
        
        // Create accounts
        NSNumber *johnsAccount = [bank createAccountForCustomer:john withInitialDeposit:500];
        NSNumber *janesAccount = [bank createAccountForCustomer:jane withInitialDeposit:1000];
        
        // Make some transactions
        [bank deposit:300 intoAccount:johnsAccount];
        [bank withdraw:200 fromAccount:johnsAccount];
        [bank deposit:500 intoAccount:janesAccount];
        [bank withdraw:1500 fromAccount:janesAccount];
        
        // Display account details
        [bank displayDetailsForAccount:johnsAccount];
        [bank displayDetailsForAccount:janesAccount];
    }
    return 0;
}
