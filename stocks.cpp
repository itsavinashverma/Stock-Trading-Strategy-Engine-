#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/**
  Best Time to Buy and Sell Stock
 * * Problem: You are given an array prices where prices[i] is the price of a given stock on the ith day.
 * You want to maximize your profit by choosing a single day to buy one stock and choosing a different
 * day in the future to sell that stock. Return the maximum profit you can achieve from this transaction.
 * If you cannot achieve any profit, return 0.
 * * Approach: This solution iterates through the price list, keeping track of the minimum price found so far (`min_price`).
 * For each day, it calculates the potential profit if we were to sell on that day (`prices[i] - min_price`)
 * and updates the maximum profit seen.
 */
class BestTimeToBuyAndSellStock_121 {
public:
    int maxProfit(const vector<int>& prices) {
        if (prices.empty()) {
            return 0;
        }

        int max_profit = 0;
        int min_price = prices[0];

        for (size_t i = 1; i < prices.size(); ++i) {
            // Calculate potential profit if selling today
            int current_profit = prices[i] - min_price;
            
            // Update the maximum profit if the current profit is higher
            if (current_profit > max_profit) {
                max_profit = current_profit;
            }

            // Update the minimum price if a new lower price is found
            if (prices[i] < min_price) {
                min_price = prices[i];
            }
        }

        return max_profit;
    }
};

/**
 Best Time to Buy and Sell Stock II
 * * Problem: You are given an integer array prices where prices[i] is the price of a given stock on the ith day.
 * On each day, you may decide to buy and/or sell the stock. You can only hold at most one share of the stock
 * at any time. However, you can buy it then immediately sell it on the same day. Find and return the maximum
 * profit you can achieve.
 * * Approach: This is a greedy approach. We can think of this as buying and selling on consecutive days
 * whenever there is a profit to be made. If the price on day `i` is greater than the price on day `i-1`,
 * we add the difference to our total profit. This is equivalent to buying on every valley and selling on
 * every peak.
 */
class BestTimeToBuyAndSellStock_II_122 {
public:
    int maxProfit(const vector<int>& prices) {
        int total_profit = 0;
        for (size_t i = 1; i < prices.size(); ++i) {
            // If the price today is higher than yesterday, we make a transaction
            if (prices[i] > prices[i - 1]) {
                total_profit += prices[i] - prices[i - 1];
            }
        }
        return total_profit;
    }
};

/**
 Best Time to Buy and Sell Stock III
 * * Problem: You are given an array prices where prices[i] is the price of a stock on the ith day.
 * Find the maximum profit you can achieve. You may complete at most two transactions.
 * Note: You may not engage in multiple transactions simultaneously (i.e., you must sell the stock
 * before you buy again).
 * * Approach: This solution uses recursion with memoization (a top-down dynamic programming approach).
 * The state is defined by the current day `i`, the number of transactions remaining `k`, and whether
 * we are currently holding a stock (`buy`).
 */
class BestTimeToBuyAndSellStock_III_123 {
private:
    int solve(int i, bool buy, int k, const vector<int>& prices, vector<vector<vector<int>>>& dp) {
        // Base case: If we've run out of days or transactions, no more profit can be made.
        if (i == prices.size() || k == 0) {
            return 0;
        }

        // If we've already computed this state, return the stored result.
        if (dp[i][k][buy] != -1) {
            return dp[i][k][buy];
        }

        int profit = 0;
        if (buy) { // We are allowed to buy
            // Option 1: Buy the stock at prices[i]. Profit is -prices[i] plus future profit.
            int buy_stock = -prices[i] + solve(i + 1, false, k, prices, dp);
            // Option 2: Don't buy today, move to the next day.
            int skip_buy = solve(i + 1, true, k, prices, dp);
            profit = max(buy_stock, skip_buy);
        } else { // We are allowed to sell
            // Option 1: Sell the stock at prices[i]. This completes one transaction (k-1).
            int sell_stock = prices[i] + solve(i + 1, true, k - 1, prices, dp);
            // Option 2: Don't sell today, move to the next day.
            int skip_sell = solve(i + 1, false, k, prices, dp);
            profit = max(sell_stock, skip_sell);
        }

        // Memoize the result and return.
        return dp[i][k][buy] = profit;
    }

public:
    int maxProfit(const vector<int>& prices) {
        if (prices.empty()) return 0;
        int n = prices.size();
        int k = 2; // At most 2 transactions
        // DP table: dp[day][transactions_left][can_buy]
        vector<vector<vector<int>>> dp(n, vector<vector<int>>(k + 1, vector<int>(2, -1)));
        return solve(0, true, k, prices, dp);
    }
};

/**
  Best Time to Buy and Sell Stock IV
 * * Problem: You are given an integer array prices where prices[i] is the price of a given stock on the ith day,
 * and an integer k. Find the maximum profit you can achieve. You may complete at most k transactions.
 * * Approach: This is a generalization of the previous problem. This solution uses tabulation
 * (a bottom-up dynamic programming approach) with space optimization. We iterate backwards from the last day.
 * The state `dp[trx][buy]` represents the max profit with `trx` transactions left, and `buy` status (1 for buy, 0 for sell).
 * We use `curr` and `after` vectors to reduce space complexity from O(n*k) to O(k).
 */
class BestTimeToBuyAndSellStock_IV_188 {
public:
    int maxProfit(int k, const vector<int>& prices) {
        int n = prices.size();
        if (n == 0 || k == 0) return 0;

        // DP tables for current and next day states
        vector<vector<int>> after(k + 1, vector<int>(2, 0));
        vector<vector<int>> curr(k + 1, vector<int>(2, 0));

        for (int i = n - 1; i >= 0; i--) {
            for (int trx = 1; trx <= k; trx++) {
                // State when we can buy
                int buy_profit = -prices[i] + after[trx][0]; // buy now
                int skip_buy_profit = after[trx][1];         // skip
                curr[trx][1] = max(buy_profit, skip_buy_profit);

                // State when we can sell
                int sell_profit = prices[i] + after[trx - 1][1]; // sell now
                int skip_sell_profit = after[trx][0];            // skip
                curr[trx][0] = max(sell_profit, skip_sell_profit);
            }
            after = curr; // Move current state to after for the next iteration
        }
        
        // The final answer is at after[k][1] (k transactions, ability to buy)
        return after[k][1];
    }
};

/**
 Best Time to Buy and Sell Stock with Cooldown
 * * Problem: You are given an array prices where prices[i] is the price of a given stock on the ith day.
 * Find the maximum profit you can achieve. You may complete as many transactions as you like, but with
 * one constraint: after you sell your stock, you cannot buy stock on the next day (i.e., cooldown of 1 day).
 * * Approach: This solution also uses recursion with memoization. The state is defined by the current day `i`
 * and whether we are in a state to buy (`buy` is true) or sell (`buy` is false). The key difference is that
 * after selling at day `i`, the next state we can buy from is `i+2`, enforcing the cooldown period.
 */
class BestTimeToBuyAndSellStock_WithCooldown_309 {
private:
    int solve(int i, bool buy, const vector<int>& prices, vector<vector<int>>& dp) {
        // Base case: If we've gone past the last day, no profit can be made.
        if (i >= prices.size()) return 0;

        // If we've already computed this state, return the stored result.
        if (dp[i][buy] != -1) return dp[i][buy];

        int profit = 0;
        if (buy) { // We are in a state to buy
            // Option 1: Buy today.
            int buy_stock = -prices[i] + solve(i + 1, false, prices, dp);
            // Option 2: Skip buying today.
            int skip_buy = solve(i + 1, true, prices, dp);
            profit = max(buy_stock, skip_buy);
        } else { // We are in a state to sell
            // Option 1: Sell today. After selling, we have a cooldown, so we jump to i+2.
            int sell_stock = prices[i] + solve(i + 2, true, prices, dp);
            // Option 2: Skip selling today.
            int skip_sell = solve(i + 1, false, prices, dp);
            profit = max(sell_stock, skip_sell);
        }

        return dp[i][buy] = profit;
    }

public:
    int maxProfit(const vector<int>& prices) {
        if (prices.empty()) return 0;
        int n = prices.size();
        // DP table: dp[day][can_buy]
        vector<vector<int>> dp(n, vector<int>(2, -1));
        return solve(0, true, prices, dp);
    }
};

// Main function to demonstrate the solutions
int main() {
    // --- Problem 121: Best Time to Buy and Sell Stock (1 transaction) ---
    cout << "--- LeetCode 121: Best Time to Buy and Sell Stock ---" << endl;
    BestTimeToBuyAndSellStock_121 sol121;
    vector<int> prices1 = {7, 1, 5, 3, 6, 4};
    cout << "Input prices: {7, 1, 5, 3, 6, 4}" << endl;
    cout << "Max Profit: " << sol121.maxProfit(prices1) << endl << endl; // Expected: 5

    // --- Problem 122: Best Time to Buy and Sell Stock II (unlimited transactions) ---
    cout << "--- LeetCode 122: Best Time to Buy and Sell Stock II ---" << endl;
    BestTimeToBuyAndSellStock_II_122 sol122;
    vector<int> prices2 = {7, 1, 5, 3, 6, 4};
    cout << "Input prices: {7, 1, 5, 3, 6, 4}" << endl;
    cout << "Max Profit: " << sol122.maxProfit(prices2) << endl << endl; // Expected: 7

    // --- Problem 123: Best Time to Buy and Sell Stock III (2 transactions) ---
    cout << "--- LeetCode 123: Best Time to Buy and Sell Stock III ---" << endl;
    BestTimeToBuyAndSellStock_III_123 sol123;
    vector<int> prices3 = {3, 3, 5, 0, 0, 3, 1, 4};
    cout << "Input prices: {3, 3, 5, 0, 0, 3, 1, 4}" << endl;
    cout << "Max Profit: " << sol123.maxProfit(prices3) << endl << endl; // Expected: 6

    // --- Problem 188: Best Time to Buy and Sell Stock IV (k transactions) ---
    cout << "--- LeetCode 188: Best Time to Buy and Sell Stock IV ---" << endl;
    BestTimeToBuyAndSellStock_IV_188 sol188;
    vector<int> prices4 = {2, 4, 1};
    int k4 = 2;
    cout << "Input prices: {2, 4, 1}, k = 2" << endl;
    cout << "Max Profit: " << sol188.maxProfit(k4, prices4) << endl << endl; // Expected: 2

    // --- Problem 309: Best Time to Buy and Sell Stock with Cooldown ---
    cout << "--- LeetCode 309: Best Time to Buy and Sell Stock with Cooldown ---" << endl;
    BestTimeToBuyAndSellStock_WithCooldown_309 sol309;
    vector<int> prices5 = {1, 2, 3, 0, 2};
    cout << "Input prices: {1, 2, 3, 0, 2}" << endl;
    cout << "Max Profit: " << sol309.maxProfit(prices5) << endl << endl; // Expected: 3

    return 0;
}
