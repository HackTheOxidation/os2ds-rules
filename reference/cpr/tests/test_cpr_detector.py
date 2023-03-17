import unittest

from ..detector.cpr_detector import CPRDetector, CPRNumber, string_to_cpr_dict


class TestCPRDetector(unittest.TestCase):

    def setUp(self) -> None:
        return super().setUp()

    def tearDown(self) -> None:
        return super().tearDown()

    def test_valid_cpr_number_with_no_separators_should_match(self):
        # Arrange
        content = "1111111118"
        detector = CPRDetector()
        expected = string_to_cpr_dict("1111111118")

        # Act
        matches = list(detector.find_matches(content))

        # Assert
        self.assertEqual(1, len(matches))

        m = matches[0]
        actual = m["cpr"]
        self.assertEqual(expected, actual)

    def test_valid_cpr_number_with_spaces_should_match(self):
        # Arrange
        content = "11 11 11 1118"
        detector = CPRDetector()
        expected = string_to_cpr_dict("1111111118")

        # Act
        matches = list(detector.find_matches(content))

        # Assert
        self.assertEqual(1, len(matches))

        m = matches[0]
        actual = m["cpr"]
        self.assertEqual(expected, actual)

    def test_valid_cpr_number_with_hyphen_should_match(self):
        # Arrange
        content = "111111-1118"
        detector = CPRDetector()
        expected = string_to_cpr_dict("1111111118")

        # Act
        matches = list(detector.find_matches(content))

        # Assert
        self.assertEqual(1, len(matches))

        m = matches[0]
        actual = m["cpr"]
        self.assertEqual(expected, actual)

    def test_cpr_number_with_excessive_space_no_match(self):
        # Arrange
        content = "111111  1118"
        detector = CPRDetector()

        # Act
        matches = list(detector.find_matches(content))

        # Assert
        self.assertEqual(0, len(matches))

    def test_cpr_number_surrounded_by_space_should_match(self):
        # Arrange
        content = " 1111111118 "
        detector = CPRDetector()

        expected_cpr = string_to_cpr_dict("1111111118")
        expected_begin = 1
        expected_end = 10

        # Act
        matches = list(detector.find_matches(content))

        # Assert
        self.assertEqual(1, len(matches))

        m = matches[0]
        actual_cpr = m["cpr"]
        self.assertEqual(expected_cpr, actual_cpr)

        actual_begin = m["begin"]
        self.assertEqual(expected_begin, actual_begin)

        actual_end = m["end"]
        self.assertEqual(expected_end, actual_end)
        
    def test_previous_not_ok_does_not_match(self):
        # Arrange
        content = "91111111118"
        detector = CPRDetector()

        # Act
        matches = list(detector.find_matches(content))

        # Assert
        self.assertEqual(0, len(matches))
