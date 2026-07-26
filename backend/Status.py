from enum import Enum

class Status(Enum):
    NOT_PLAYED = 1
    STARTED = 2
    COMPLETED = 3

    @staticmethod
    def convert(status: str) -> None | Status:
        if status == "Not Played":
            return Status.NOT_PLAYED
        elif status == "Started":
            return Status.STARTED
        elif status == "Completed":
            return Status.COMPLETED
        else:
            return None