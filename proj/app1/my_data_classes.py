from pathlib import Path

import json
from dataclasses import dataclass, field

SENSORS_FILE_NAME = 'sensors.json'
RELAYS_FILE_NAME = 'relays.json'


@dataclass()
class Sensors:
    @dataclass
    class Data:
        time : str = None
        temp : float = None
        humid : float = None
        lpg : float = None
        smk : float = None

        def is_valid(self):
            err_resp = {}
            result = True

            for field, field_type in self.__annotations__.items():
                if not isinstance(getattr(self, field), field_type):
                    try:
                        casted_value = field_type(getattr(self, field))
                    except Exception as e:
                        # print(e)
                        err_resp[field] = '[field is required]'
                        result = False
                    else:
                        setattr(self, field, casted_value)

            return result, err_resp
    path : str
    data : dict = field(default_factory=dict)

    def __post_init__(self):
        self.path = Path(self.path)
        self.data = Sensors.Data(**self.data)
        print('>>', self.data, self.data.is_valid())

    def is_valid(self):
        return self.data.is_valid()
    
    def save(self, file_name=SENSORS_FILE_NAME):
        with open((self.path / file_name), 'w+') as fp:
            print(self.data.__dict__)
            json.dump(self.data.__dict__, fp)

    def read(self, file_name=SENSORS_FILE_NAME):
        with open((self.path / file_name), 'r') as fp:
            data = json.load(fp)
            self.data = Sensors.Data(**data)
        return data
# 4$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


@dataclass()
class Relays:
    @dataclass
    class Data:
        rel1 : bool = False
        rel2 : bool = False
        rel3 : bool = False
        rel4 : bool = False

        def is_valid(self):
            err_resp = {}
            result = True

            for field, field_type in self.__annotations__.items():
                if not isinstance(getattr(self, field), field_type):
                    try:
                        casted_value = field_type(getattr(self, field))
                    except Exception as e:
                        # print(e)
                        err_resp[field] = '[field is required]'
                        result = False
                    else:
                        setattr(self, field, casted_value)

            return result, err_resp
    path : str
    data : dict = field(default_factory=dict)

    def __post_init__(self):
        self.path = Path(self.path)
        self.data = Relays.Data(**self.data)
        print('>>', self.data, self.data.is_valid())

    def is_valid(self):
        return self.data.is_valid()
    
    def save(self, file_name=RELAYS_FILE_NAME):
        with open((self.path / file_name), 'w+') as fp:
            print(self.data.__dict__)
            json.dump(self.data.__dict__, fp)

    def read(self, file_name=RELAYS_FILE_NAME):
        # print(self.path / file_name)
        with open((self.path / file_name), 'r') as fp:
            data = json.load(fp)
            self.data = __class__.Data(**data)
        return data