type countryCode =
  | GB
  | US
  | JP
  | SG
  | CH;

type currency =
  | GBP
  | USD
  | JPY
  | SGD
  | CHF;

type epoch =
  | Epoch(int);

type salary =
  | Undiscolsed
  | Totoal(int)
  | Range(int, int);

type jobType =
  | Permenant
  | Contract;

type rate =
  | Yearly
  | Daily;

type jobId =
  | JobId(string);

type job = {
  jobId,
  jobTitle: string,
  companyName: string,
  keyWords: string,
  jobDescription: string,
  ancillarySkills: list(string),
  auxillarySkills: option(list(string)),
  aditionalInformation: option(string),
  remote: bool,
  countryCode,
  salary,
  currency,
  rate,
  postingDate: epoch, // unix timestamp, perhaps we should make a type for that...
  closingDate: epoch // unix timestamp, perhaps we should make a type for that...
};