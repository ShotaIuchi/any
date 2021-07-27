// thread_test1.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <thread>
#include <functional>
#include <cassert>
#include <chrono>
#include <atomic>
#include <future>

class ABCManager {
    std::atomic_int count = 0;
public:
    void a() {
        count++;
        std::cout << "ABCManager::a!" << count << "\n";
    }
    void b() {
        count++;
        std::cout << "ABCManager::b!" << count << "\n";
    }
};

class Thread1
{
    ABCManager& abc_;
public:
    Thread1(ABCManager& abc)
        : thread_(main_)
        , abc_(abc)
        , isTerminationRequested_(false)
    {}
    ~Thread1() {
        assert(thread_.joinable());
        isTerminationRequested_ = true;
        thread_.join();
    }
    void requestTermination() {
        isTerminationRequested_ = true;
    }
private:
    std::function<void()> main_ = [this]
    {
        while (1) {
            //std::cout << "1!\n";
            abc_.a();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            // Do Something
            if (isTerminationRequested_) { return; }
        }
    };
public:
    bool isTerminationRequested_;
    std::thread thread_;
};

class Thread2
{
    ABCManager& abc_;
public:
    Thread2(ABCManager& abc)
        : thread_(main_)
        , abc_(abc)
        , isTerminationRequested_(false)
    {}
    ~Thread2() {
        assert(thread_.joinable());
        isTerminationRequested_ = true;
        thread_.join();
    }
    void requestTermination() {
        isTerminationRequested_ = true;
    }
private:
    std::function<void()> main_ = [this]
    {
        while (1) {
            //std::cout << "2!\n";
            abc_.b();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            // Do Something
            if (isTerminationRequested_) { return; }
        }
    };
public:
    bool isTerminationRequested_;
    std::thread thread_;
};

ABCManager abc___;













class SIThread
{
public:
    SIThread() : mIsRequestedTerminat(false), mIsTerminated(false), mThread(NULL)
    {
        mThread = new std::thread(runner());
    }
    virtual ~SIThread() {
        assert(mThread->joinable());
        mIsRequestedTerminat = true;
        mThread->join();
    }
    void requestTermination() {
        mIsRequestedTerminat = true;
    }
    bool isRequestedTerminat() {
        return mIsRequestedTerminat;
    }
    bool isTerminated() {
        return mIsTerminated;
    }
protected:
    virtual void run() = 0;
private:
    std::function<void()> runner() {
        return [this]
        {
            while (!isRequestedTerminat()) {
                run();
            }
            mIsTerminated = true;
        };
    }
    bool mIsRequestedTerminat;
    bool mIsTerminated;
    std::thread* mThread;
};

class SIThreadEX : public SIThread {
public:
    SIThreadEX(ABCManager& abc) : mAbc(abc) {}
    virtual ~SIThreadEX() {}
protected:
    virtual void run() override {
        mAbc.a();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
private:
    ABCManager& mAbc;
};

class SIThreadEXPool : public SIThread {
public:
    SIThreadEXPool(ABCManager& abc) : mAbc(abc) {}
    virtual ~SIThreadEXPool() {}
protected:
    virtual void run() override {
        mAbc.a();
        a++;
        auto f1 = std::async(std::launch::async, [&]() {
            for (int i = 0; i < 2; i++) {
                std::cout << "!!" << a << "!!\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        });
        /*auto f2 = std::async(std::launch::async, [&]() {
            for (int i = 0; i < 2; i++) {
                std::cout << "!!" << a+20 << "!!\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
            });*/
        //f1.get();
        //f2.get();
        std::cout << "!!FIX!!\n";
        //requestTermination();
    }
private:
    int a = 0;
    ABCManager& mAbc;
};


int main()
{
    SIThreadEXPool t(abc___);
    while (!t.isTerminated()) {
        /*char a = getchar();
        if (a == '1') {
            t.requestTermination();
        }*/
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    /*Thread1 t1(abc___);
    Thread2 t2(abc___);
    SIThreadEX t3(abc___);

    while (!t1.isTerminationRequested_ || !t2.isTerminationRequested_ || !t3.isTerminated()) {
        char a = getchar();
        if (a == '1') {
            t1.requestTermination();
        }
        if (a == '2') {
            t2.requestTermination();
        }
        if (a == '3') {
            t3.requestTermination();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }*/
    std::cout << "Hello World!\n";
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
