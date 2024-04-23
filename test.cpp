#include <gtest/gtest.h>
#include <thread>
#include "lib/lmpt.h"

TEST(Lamport, Constructor) {
    Lamport l(1, 8080);
    EXPECT_EQ(l.getProcessId(), 1);
    EXPECT_EQ(l.getServerPort(), 8080);
}

// Add more tests as needed

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
