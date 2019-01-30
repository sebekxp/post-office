Markup :  # Modern post office #
Markup : * It manages pending clients in a fair manner on the principle of downloading numbers, with</br>
the possibility of handling, for example, pregnant women or disabled people outside the queue.</br>
Markup : * Provides remote access to pending shipments</br>
Markup : * Verifies the identity of customers based on biometric data instead of an identity document</br>
Markup : * It stores data in a persistent manner</br></br>
Markup :  ### Example of use:<br/><br/> ###

```cpp
try {
        auto post_office = IPostOffice::create(5);

        auto client0 = post_office->getClient("96052791813");
        client0->setFullName("Random Name");
        client0->newPackage("pack1");
        client0->newPackage("pack2");

        auto client1 = post_office->getClient("93052753513");
        client1->setFullName("Random Name1");
        client1->updatePriority(1);
        client1->newPackage("pack1");
        client1->newPackage("pack2");
        client1->updateBiometricData("GGTTGACTA");

        post_office->enqueueClient(client0);
        post_office->enqueueClient(client1);

        std::cout << client0->verifyBiometricData("TGTTACGG", 2.0) << std::endl;
        std::cout << client1->verifyBiometricData("TGTTACGG", 2.0) << std::endl;

        post_office->gateReady(3);
        auto status = post_office->getStatus();
        assert(status[3] == "Random Name1");

        post_office->gateReady(3);
        status = post_office->getStatus();
        assert(status[3] == "Random Name");

    } catch (const IncorrectGateException &e) {
        std::cout << "IncorrectGateException: " << e.what() << std::endl;
    } catch (const PackageExistsException &e) {
        std::cout << "PackageExistsException: " << e.what() << std::endl;
    } catch (IncorrectBiometricDataException &e) {
        std::cout << "IncorrectBiometricDataException: " << e.what() << std::endl;
    }
```
