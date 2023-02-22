#include <cpr-detector.hpp>


namespace CPRDetector {

  constexpr CPRDetector::CPRDetector(CPRDetector&& other) noexcept
    : state_(std::move(other.state_)), check_mod11_(std::move(other.check_mod11_)) {}

  constexpr CPRDetector& CPRDetector::operator=(CPRDetector&& other) noexcept {
    if (this != &other) {
      std::swap(other.check_mod11_, check_mod11_);
      state_ = std::move(other.state_);
    }

    return *this;
  }

  void CPRDetector::reset() noexcept {
    state_ = CPRDetectorState::Empty;
  }

  char CPRDetector::update(char c, CPRDetectorState new_state, Predicate is_acceptable) noexcept {
    if (is_acceptable(c)) {
      state_ = new_state;
      return c;
    } else {
      reset();
      return 0;
    }
  }

  bool CPRDetector::check_day_month(const std::string& cpr) noexcept {
    int day = std::stoi(std::string(cpr[0], cpr[1]));
    int month = std::stoi(std::string(cpr[2], cpr[3]));

    if (month == 2) {
      if (day == 29)
	return true;
      else if (day > 29)
	reset();
    } else if (day > 30 &&
	       !((month > 7 && month % 2 == 0)
		|| (month < 8 && month % 2 != 0))) {
      reset();
    } 
    
    return false;
  }

  void CPRDetector::check_leap_year(const std::string& cpr) noexcept {
    int year = std::stoi(std::string(cpr[4], cpr[5]));

    if (year % 4 != 0)
      reset();
  }

  void CPRDetector::check_and_append_cpr(std::string& cpr, CPRResults& results, size_t begin, size_t end) noexcept {
    int control = std::stoi(std::string(cpr[6], cpr[9]));

    if (control > 0) {
      CPRResult result(cpr, begin, end);
      results.push_back(result);
    }

    cpr = "";
  }

  CPRResults CPRDetector::find_matches(const std::string& content) noexcept {
    CPRResults results{};

    auto content_length = content.size();

    if (content_length < 10) {
      return results;
    }

    // TODO: find a reasonable data type to store a CPR-number.
    std::string cpr(10, 0);
    char previous = 0;
    size_t begin = 0;
    bool allow_separator = false;
    bool leap_year = false;
    Predicate is_acceptable = [](char) { return false; };

    for(auto it = std::begin(content); it != std::end(content); ++it) {
      switch (state_) {
      case CPRDetectorState::Empty:
	is_acceptable = make_predicate('0', '1', '2', '3');
	previous = update(*it, CPRDetectorState::First, is_acceptable);

	if (previous != 0) {
	  cpr[0] = previous;
	  begin = std::distance(content.begin(), it);
	}

	break;
      case CPRDetectorState::First:
	if (previous == '0') {
	  is_acceptable = is_nonzero_digit;
	} else if (previous == '1' || previous == '2') {
	  is_acceptable = is_digit;
	} else if (previous == '3') {
	  is_acceptable = make_predicate('0', '1');
	} else {
	  reset();
	  previous = 0;
	  continue;
	}

	previous = cpr[1] = update(*it, CPRDetectorState::Second, is_acceptable);

	if (previous != 0)
	  allow_separator = true;

	break;
      case CPRDetectorState::Second:
	if (is_space(*it) && allow_separator) {
	  allow_separator = false;
	  continue;
	}

	is_acceptable = make_predicate('0', '1');
	previous = cpr[2] = update(*it, CPRDetectorState::Third, is_acceptable);

	break;
      case CPRDetectorState::Third:
	if (previous == '0') {
	  is_acceptable = is_nonzero_digit;
	} else if (previous == '1') {
	  is_acceptable = make_predicate('0', '1', '2');
	} else {
	  reset();
	  previous = 0;
	  continue;
	}

	previous = cpr[3] = update(*it, CPRDetectorState::Fourth, is_acceptable);

	leap_year = check_day_month(cpr);

	if (previous != 0)
	  allow_separator = true;

	break;
      case CPRDetectorState::Fourth:
	if (is_space(*it) && allow_separator) {
	  allow_separator = false;
	  continue;
	}

	is_acceptable = is_digit;

	previous = cpr[4] = update(*it, CPRDetectorState::Fifth, is_acceptable);
	
	break;
      case CPRDetectorState::Fifth:
	if (previous == '0') {
	  is_acceptable = is_nonzero_digit;
	} else if (previous == '1') {
	  is_acceptable = is_digit;
	} else {
	  reset();
	  previous = 0;
	  continue;
	}

	previous = cpr[5] = update(*it, CPRDetectorState::Sixth, is_acceptable);

	if (leap_year) 
	  check_leap_year(cpr);
       
	if (previous != 0) 
	  allow_separator = true;
	
	break;
      case CPRDetectorState::Sixth:
	if (allow_separator && is_separator(*it)) {
	  allow_separator = false;
	  continue;
	}

	is_acceptable = is_digit;
	previous = cpr[6] = update(*it, CPRDetectorState::Seventh, is_acceptable);
	
	break;
      case CPRDetectorState::Seventh:
	is_acceptable = is_digit;
	previous = cpr[7] = update(*it, CPRDetectorState::Eighth, is_acceptable);

	break;
      case CPRDetectorState::Eighth:
	is_acceptable = is_digit;
	previous = cpr[8] = update(*it, CPRDetectorState::Match, is_acceptable);

	break;
      case CPRDetectorState::Match:
	is_acceptable = is_digit;
	previous = cpr[9] = update(*it, CPRDetectorState::Match, is_acceptable);

	check_and_append_cpr(cpr, results, begin, std::distance(content.begin(), it));

	previous = 0;
	reset();

	break;
      default:
	reset();
	break;
      }
    }

    return results;
  }
};

