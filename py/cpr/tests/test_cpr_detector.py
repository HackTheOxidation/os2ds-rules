import unittest

from ..detector.cpr_detector import CPRDetector


class TestCPRDetector(unittest.TestCase):

    def setUp(self) -> None:
        return super().setUp()

    def tearDown(self) -> None:
        return super().tearDown()

    def test_valid_cpr_number_with_no_separators_should_match(self):
        # 
        content = "1111111118"
        detector = CPRDetector()

        #
        matches = list(detector.find_matches(content))

        # Assert
        self.assertEqual(1, len(matches))

        expected = "1111111118"
        actual = str(matches[0]["cpr"])
        self.assertEqual(expected, actual)

