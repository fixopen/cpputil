#ifndef __UTIL_LOG_H__
#define __UTIL_LOG_H__

#include <vector>
#include <string>
#include "Text/StringOp.h"

namespace Util {
    namespace Log {
        struct Entry;
        class Processor;
            class Source;
            class Sink;
            class Filter;
            //class Forker;
            //class Decorate;
                class TimeDecorate;
        class Receiver;
            class NetworkReceiver;
        class Formatter;
            class SQLFormatter;
        class Parser;
            class SQLParser;
        class Destination;
            class FileDestination;
            class TraceDestination;
            class UIDestination;
            class MemoryDestination;
            class NetworkDestination;
            class DatabaseDestination;

        int const AddSource(Source* const processor);
        int const AddProcess(Processor* const processor, int const previousNode);
        Processor* const RemoveProcess(int const id);
        //int const AddSource(Source* const source, Parser* const parser, Receiver* const receiver);
        //int const AddSink(Sink* const sink, Formatter* const formatter, Destination* const destination);
        void Log(Entry const& entry);
        void StartLog();
        Processor* const FindNodeById(Processor* const node, int const id);

        struct Entry {
            enum Level {
                lFatalError,
                lError,
                lWarning,
                lNotice,
                lInfo,
            };

            Entry(std::string const& w, ...)
            : level(lNotice) {
                std::string r = w;
                char const* format = r.c_str();
                char buffer[4096] = {0};
                va_list ap;
                va_start(ap, format);
                int len = vsprintf_s(buffer, 4096, format, ap);
                //@@va_end(ap);
                r = buffer;
                what = r;
            }

            Entry(Level const lvl, std::string const& w, ...)
            : level(lvl) {
                std::string r = w;
                char const* format = r.c_str();
                char buffer[4096] = {0};
                va_list ap;
                va_start(ap, format);
                int len = vsprintf_s(buffer, 4096, format, ap);
                //@@va_end(ap);
                r = buffer;
                what = r;
            }

            Entry(std::wstring const& w)
            : level(lNotice)
            , what(Text::StringOp::FromUnicode(w)) {
            }

            Entry(char const* const w)
            : level(lNotice)
            , what(w) {
            }

            Entry(wchar_t const* const w)
            : level(lNotice)
            , what(Text::StringOp::FromUnicode(w)) {
            }

            Level level;
            std::string what;
        };
        class Processor {
            friend int const AddSource(Source* const processor);
            friend int const AddProcess(Processor* const processor, int const previousNode);
            friend Processor* const RemoveProcess(int const id);
            //friend int const AddSource(Source* const source, Parser* const parser, Receiver* const receiver);
            //friend int const AddSink(Sink* const sink, Formatter* const formatter, Destination* const destination);
            friend Processor* const FindNodeById(Processor* const node, int const id);
        public:
            virtual Entry const Process(Entry const& entry);
            void AddNext(Processor* const next);
            void RemoveNext(Processor* const next);
            virtual ~Processor();
        protected:
            std::vector<Processor*> nexts_;
            int id_;
        };
        class Source : public Processor {
        public:
            virtual Entry const Process(Entry const& entry);
            virtual void SetParser(Parser* const parser);
            virtual void SetReceiver(Receiver* const receiver);
            virtual ~Source();
        private:
            Parser* parser_;
            Receiver* receiver_;
        };
        class Sink : public Processor {
        public:
            virtual Entry const Process(Entry const& entry);
            virtual void SetFormatter(Formatter* const formatter);
            virtual void SetDestination(Destination* const destination);
            virtual ~Sink();
        private:
            Formatter* formatter_;
            Destination* destination_;
        };
        class Filter : public Processor {
        public:
            virtual bool const IsAccept(Entry const& entry);
            virtual void SetLevel(Entry::Level const level);
            virtual ~Filter();
        private:
            Entry::Level level_;
        };
        //class Forker : public Processor { //same as processor
        //public:
        //    virtual ~Forker();
        //};
        class Receiver {
        public:
            virtual std::string const Read();
        };
        class NetworkReceiver : public Receiver {
        public:
            NetworkReceiver(std::string const& ip, unsigned short const port);
            virtual std::string const Read();
        private:
            typedef int Socket;
            Socket socket_;
        };
        class Formatter {
        public:
            virtual std::string const Format(Entry const& entry);
        };
        class SQLFormatter : public Formatter {
        public:
            virtual std::string const Format(Entry const& entry);
        };
        class Parser {
        public:
            virtual Entry const Parse(std::string const& value);
        };
        class SQLParser : public Parser {
        public:
            virtual Entry const Parse(std::string const& sql);
        };
        class Destination {
        public:
            virtual void Write(std::string const& value);
        };
        class FileDestination : public Destination {
        public:
            FileDestination(std::string const& filename = "");
            ~FileDestination();
            virtual void Write(std::string const& value);
        private:
            FILE* file_;
            bool isConsole_;
        };
        class TraceDestination : public Destination {
        public:
            virtual void Write(std::string const& value);
        };
        class UIDestination : public Destination {
        public:
            virtual void Write(std::string const& value);
        };
        class MemoryDestination : public Destination {
        public:
            MemoryDestination(size_t count);
            virtual void Write(std::string const& value);
        private:
            std::vector<std::string> contents_;
        };
        class NetworkDestination : public Destination {
        public:
            NetworkDestination(std::string const& ip, unsigned short const port);
            virtual void Write(std::string const& value);
        private:
            typedef int Socket;
            Socket* socket_;
        };
        class DatabaseDestination : public Destination {
        public:
            DatabaseDestination(std::string const& databaseName, std::string const& tableName);
            virtual void Write(std::string const& value);
        private:
            typedef int DB;
            DB* database_;
        };
    }
}

#endif //__UTIL_LOG_H__
