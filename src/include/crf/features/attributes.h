/**
 * attributes.h.
 * Defines the attributes object, which is used in training.
 */
namespace NLP {
  namespace CRF {
    namespace HT = Util::hashtable;

    class Attributes {
      private:
        class Impl;
        Impl *_impl;

      public:
        Attributes(const size_t nbuckets=HT::MEDIUM,
            const size_t pool_size=HT::LARGE);
        Attributes(const std::string &filename, const size_t nbuckets=HT::MEDIUM,
            const size_t pool_size=HT::LARGE);
        Attributes(const std::string &filename, std::istream &input,
            const size_t nbuckets=HT::MEDIUM, const size_t pool_size=HT::LARGE);
        Attributes(const Attributes &other);

        ~Attributes(void);

        void load(const std::string &filename);
        void load(const std::string &filename, std::istream &input);
        void save_attributes(const std::string &filename, const std::string &preface);
        void save_attributes(std::ostream &out, const std::string &preface);
        void save_features(const std::string &filename, const std::string &preface);
        void save_features(std::ostream &out, const std::string &preface);

        void operator()(const char *type, const std::string &str, TagPair &tp, const bool add_state_feature=true, const bool add_trans_feature=true);
        void operator()(const char *type, const std::string &str, Context &c);
        void sort_by_freq(void);
        void reset_expectations(void);
        void reset(void);

        void adagrad_update(double *weights, double *history, const double t0,
            const double inv_sigma_sq, const double inv_n);

        uint64_t nfeatures(void) const;

        void apply_attrib_cutoff(const uint64_t freq);
        void apply_cutoff(const uint64_t freq);
        void apply_cutoff(const Type &type, const uint64_t freq);
        void apply_cutoff(const Type &type, const uint64_t freq, const uint64_t def);

        double sum_lambda_sq(void);
        void assign_lambdas(double *x);
        void zero_lambdas(void);
        void copy_gradients(double *x, double inv_sigma_sq);
        bool inc_next_lambda(double val);
        void print_current_gradient(double val, double inv_sigma_sq);
        void print(double inv_sigma_sq);
        void print_adagrad(double inv_sigma_sq, double inv_n);
        void prep_finite_differences(void);
        void load_trans_features(const char *type, const std::string &str);
        FeaturePtrs &trans_features(void);

        size_t size(void) const;
    };
  }

}
